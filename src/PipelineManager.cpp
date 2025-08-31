#include "tp_pipeline/PipelineManager.h"
#include "tp_pipeline/StepDelegate.h"
#include "tp_pipeline/StepDelegateMap.h"
#include "tp_pipeline/PipelineDetails.h"

#include "tp_data/Collection.h"
#include "tp_data/CollectionFactory.h"

#include <unordered_set>

namespace tp_pipeline
{

//##################################################################################################
bool StepContext::dependenciesMet() const
{
  for(auto s : dependsOn)
    if(!s->runComplete)
      return false;

  return true;
}

//##################################################################################################
struct PipelineManager::Private
{
  Q* q;
  PipelineDetails* pipelineDetails;
  const tp_pipeline::StepDelegateMap* stepDelegates;
  const tp_data::CollectionFactory* collectionFactory;

  std::vector<StepContext> stepContexts;

  std::vector<StepContext*> requiredSteps;

  bool ignoreCallbacks{false};

  //################################################################################################
  Private(Q* q_,
          PipelineDetails* pipelineDetails_,
          const tp_pipeline::StepDelegateMap* stepDelegates_,
          const tp_data::CollectionFactory* collectionFactory_,
          bool fixupParameters):
    q(q_),
    pipelineDetails(pipelineDetails_),
    stepDelegates(stepDelegates_),
    collectionFactory(collectionFactory_)
  {
    rebuildContexts(fixupParameters);
  }

  //################################################################################################
  tp_utils::Callback<void(StepDetails*)> changedCallback = [&](StepDetails* stepDetails)
  {
    TP_UNUSED(stepDetails);
    if(ignoreCallbacks)
      return;

    rebuildContexts();
  };

  //################################################################################################
  tp_utils::Callback<void(StepDetails*)> deletedCallback = [&](StepDetails* stepDetails)
  {
    TP_UNUSED(stepDetails);
    if(ignoreCallbacks)
      return;

    rebuildContexts();
  };

  //################################################################################################
  size_t indexOf(StepDetails* stepDetails)
  {
    for(size_t i=0; i<stepContexts.size(); i++)
      if(stepContexts.at(i).stepDetails == stepDetails)
        return i;

    return 0;
  }

  //################################################################################################
  StepContext* stepContextFor(StepDetails* stepDetails)
  {
    for(auto& stepContext : stepContexts)
      if(stepContext.stepDetails == stepDetails)
        return &stepContext;
    return nullptr;
  }

  //################################################################################################
  void rebuildContexts(bool fixupParameters=true)
  {
    const auto& steps = pipelineDetails->steps();
    requiredSteps.clear();
    requiredSteps.reserve(steps.size());
    stepContexts.resize(steps.size());
    for(size_t i=0; i<stepContexts.size(); i++)
    {
      auto stepDetails = steps.at(i);
      auto& stepContext = stepContexts.at(i);

      stepContext.stepDetails = stepDetails;
      stepContext.stepDelegate = stepDelegates->stepDelegate(stepDetails->delegateName());

      if(stepContext.stepDelegate)
      {
        stepContext.stepInput = std::make_shared<tp_data::Collection>();
        stepContext.stepOutput = std::make_shared<StepOutput>(stepContext.stepDelegate, stepContext.stepDetails);

        if(fixupParameters)
        {
          ignoreCallbacks=true;
          stepContext.stepDelegate->fixupParameters(stepDetails);
          ignoreCallbacks=false;
        }
      }
    }
  }

  //################################################################################################
  void resetAndBuildDependencies()
  {
    for(auto& stepContext : stepContexts)
    {
      stepContext.dependsOn.clear();

      std::unordered_set<tp_utils::StringID> requiredData;
      for(const auto& m : stepContext.stepDetails->inputMapping())
        if(m.dataName.isValid())
          requiredData.insert(m.dataName);

      for(auto& s : stepContexts)
      {
        for(const auto& m : s.stepDetails->outputMapping())
        {
          if(tpContains(requiredData, m.dataName))
          {
            stepContext.dependsOn.insert(&s);
            break;
          }
        }
      }
    }
  }

  //################################################################################################
  void addStepAndDependencies(StepContext* stepContext)
  {
    if(tpContains(requiredSteps, stepContext))
      return;

    requiredSteps.push_back(stepContext);

    for(auto s : stepContext->dependsOn)
      addStepAndDependencies(s);
  }

  //################################################################################################
  void addAllSteps()
  {
    requiredSteps.clear();
    for(auto& s : stepContexts)
      requiredSteps.push_back(&s);
  }
};

//##################################################################################################
PipelineManager::PipelineManager(PipelineDetails* pipelineDetails,
                                 const tp_pipeline::StepDelegateMap* stepDelegates,
                                 const tp_data::CollectionFactory* collectionFactory,
                                 bool fixupParameters):
  d(new Private(this, pipelineDetails, stepDelegates, collectionFactory, fixupParameters))
{
  d->changedCallback.connect(d->pipelineDetails->changedCallback);
  d->deletedCallback.connect(d->pipelineDetails->deletedCallback);
}

//##################################################################################################
PipelineManager::~PipelineManager()
{
  delete d;
}

//##################################################################################################
PipelineDetails* PipelineManager::pipelineDetails() const
{
  return d->pipelineDetails;
}

//##################################################################################################
void PipelineManager::startExecution(StepDetails* finalStep)
{
  d->resetAndBuildDependencies();
  if(finalStep)
  {
    if(auto s=d->stepContextFor(finalStep); s)
      d->addStepAndDependencies(s);
  }
  else
    d->addAllSteps();
}

//##################################################################################################
StepContext* PipelineManager::takeNextAvailableStep()
{
  for(auto s : d->requiredSteps)
  {
    if(s->runStarted==false && s->dependenciesMet())
    {
      s->runStarted = true;

      // Gather the input data here...

      return s;
    }
  }

  return nullptr;
}

//##################################################################################################
void PipelineManager::returnCompletedStep(StepContext* stepContext)
{
  stepContext->runComplete = true;
}

//##################################################################################################
StepContext* PipelineManager::stepContext(StepDetails* stepDetails) const
{
  return d->stepContextFor(stepDetails);
}

}
