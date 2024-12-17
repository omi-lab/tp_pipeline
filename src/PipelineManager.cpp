#include "tp_pipeline/PipelineManager.h"
#include "tp_pipeline/AbstractStepDelegate.h"
#include "tp_pipeline/StepDelegateMap.h"
#include "tp_pipeline/PipelineDetails.h"

#include "tp_data/Collection.h"
#include "tp_data/CollectionFactory.h"

#include "tp_utils/StackTrace.h"
#include "tp_utils/DebugUtils.h"

namespace tp_pipeline
{

namespace
{
//##################################################################################################
struct StepDetails_lt
{
  StepDetails* stepDetails{nullptr};

  const AbstractStepDelegate* delegate{nullptr};

  std::shared_ptr<tp_data::Collection> results;
  std::shared_ptr<tp_data::Collection> persistentData;

  //Set true when the cached data is valid
  bool cacheValid{false};

  StepDetails_lt():
    results(new tp_data::Collection())
  {

  }
};
}

//##################################################################################################
struct PipelineManager::Private
{
  Q* q;
  PipelineDetails* pipelineDetails;
  PipelineDetails dummyPipeline;
  const tp_pipeline::StepDelegateMap* stepDelegates;
  const tp_data::CollectionFactory* collectionFactory;

  std::vector<StepDetails_lt> cachedState;

  tp_utils::Callback<void(StepDetails*)> changedCallback;
  tp_utils::Callback<void(StepDetails*)> deletedCallback;

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
    collectionFactory(collectionFactory_),
    changedCallback([&](StepDetails* stepDetails){stepChanged(stepDetails);}),
    deletedCallback([&](StepDetails* stepDetails){stepDeleted(stepDetails);})
  {
    if(!pipelineDetails)
    {
      pipelineDetails = &dummyPipeline;
      tpWarning() << "Created PipelineManager with nullptr pipeline.";
      tp_utils::printStackTrace();
    }

    std::string error;
    updateRoot(error, std::make_shared<tp_data::Collection>());
    rebuildCache(fixupParameters);
  }

  //################################################################################################
  size_t indexOf(StepDetails* stepDetails)
  {
    for(size_t i=0; i<cachedState.size(); i++)
      if(cachedState.at(i).stepDetails == stepDetails)
        return i;

    return 0;
  }

  //################################################################################################
  std::vector<std::string> execute(size_t index, StepInput& input, size_t skip=0)
  {
    std::vector<std::string> errors;

    if(index>=cachedState.size())
    {
      errors.push_back("Invalid index!");
      return errors;
    }

    bool forceInvalidate=false;
    for(size_t i=skip; i<=index; i++)
    {
      StepDetails_lt& stepDetails = cachedState[size_t(i)];

      //If we need to and can execute the step
      if((!stepDetails.cacheValid || forceInvalidate))
      {
        forceInvalidate=true;

        if(stepDetails.stepDetails)
        {
          if(stepDetails.delegate)
          {
            stepDetails.results = std::make_shared<tp_data::Collection>();

            if(!stepDetails.persistentData)
              stepDetails.persistentData = std::make_shared<tp_data::Collection>();

            stepDetails.delegate->executeStep(stepDetails.stepDetails,
                                              input,
                                              *stepDetails.results,
                                              *stepDetails.persistentData);

            errors.insert(errors.end(), stepDetails.results->errors().begin(), stepDetails.results->errors().end());
          }
          else
          {
            auto error = "Failed to find step delegate for: " + stepDetails.stepDetails->delegateName().toString();
            errors.push_back(error);
            tpWarning() << error;
          }
        }

        stepDetails.cacheValid = true;
      }

      if(stepDetails.results)
        input.previousSteps.push_back(stepDetails.results);
    }

    return errors;
  }

  //################################################################################################
  //This updates the imaginary item at the start of the pipeline that is used to feed data into the
  //pipeline.
  void updateRoot(std::string& error, const std::shared_ptr<tp_data::Collection>& input)
  {
    if(cachedState.empty())
      cachedState.push_back(StepDetails_lt());

    StepDetails_lt& root = cachedState[0];
    root.results.reset(new tp_data::Collection);
    collectionFactory->cloneAppend(error, *input, *root.results);

    root.cacheValid = false;
  }

  //################################################################################################
  void rebuildCache(bool fixupParameters=true)
  {
    //-- Create and delete cache entries as required -----------------------------------------------
    bool changed=false;
    {
      size_t i=1;
      for(StepDetails* step : pipelineDetails->steps())
      {
        if(i<cachedState.size())
        {
          if(cachedState[i].stepDetails != step)
          {
            changed=true;
            while(cachedState.size()>i)
              cachedState.erase(cachedState.end()-1);
          }
        }

        if(i>=cachedState.size())
        {
          changed=true;
          StepDetails_lt details;
          details.stepDetails = step;
          cachedState.push_back(details);
        }

        i++;
      }
    }

    //-- If anything has changed fixup the delegates -----------------------------------------------
    if(changed)
    {
      for(size_t i=1; i<cachedState.size(); i++)
      {
        StepDetails_lt& step = cachedState[i];
        step.cacheValid = false;
        step.delegate = nullptr;

        if(step.stepDetails)
        {
          auto stepDelegate = stepDelegates->stepDelegate(step.stepDetails->delegateName());
          if(stepDelegate)
          {
            step.delegate = stepDelegate;
            if(fixupParameters)
            {
              ignoreCallbacks=true;
              step.delegate->fixupParameters(step.stepDetails);
              ignoreCallbacks=false;
            }
          }
        }
      }
    }
  }

  //################################################################################################
  void stepChanged(StepDetails* stepDetails)
  {
    if(ignoreCallbacks)
      return;

    rebuildCache();

    size_t index = indexOf(stepDetails);

    if(index<cachedState.size())
    {
      //Invalidate the cache on steps after this one
      for(size_t i=size_t(index+1); i<cachedState.size(); i++)
        cachedState[i].cacheValid = false;

      StepDetails_lt& step = cachedState[size_t(index)];
      step.cacheValid = false;
      auto stepDelegate = stepDelegates->stepDelegate(stepDetails->delegateName());
      if(stepDelegate)
      {
        step.delegate = stepDelegate;
        ignoreCallbacks=true;

        step.delegate->fixupParameters(stepDetails);
        ignoreCallbacks=false;
        rebuildCache();
      }
    }
  }

  //################################################################################################
  void stepDeleted(StepDetails* stepDetails)
  {
    if(ignoreCallbacks)
      return;

    size_t index = indexOf(stepDetails);
    for(size_t i=index; i<cachedState.size(); i++)
      cachedState[i].cacheValid=false;

    rebuildCache();
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
std::shared_ptr<tp_data::Collection> PipelineManager::execute(std::vector<std::string>& errors, const std::shared_ptr<tp_data::Collection>& input)
{
  {
    std::string error;
    d->updateRoot(error, input);
    if(!error.empty())
    {
      errors.push_back(error);
      return {};
    }
  }

  size_t lastIndex = d->cachedState.size()-1;

  StepInput rollingInput;
  errors = d->execute(lastIndex, rollingInput);
  return d->cachedState.at(lastIndex).results;
}

//##################################################################################################
std::shared_ptr<tp_data::Collection> PipelineManager::executeFrom(std::vector<std::string>& errors, const StepInput& input, size_t skip)
{
  size_t lastIndex = d->cachedState.size()-1;

  StepInput rollingInput;
  rollingInput.previousSteps = input.previousSteps;

  //There is a dummy step that is used to feed data into the pipeline so add 1 to skip
  errors = d->execute(lastIndex, rollingInput, skip+1);
  return d->cachedState.at(lastIndex).results;
}


//##################################################################################################
std::shared_ptr<tp_data::Collection> PipelineManager::executeStep(StepDetails* stepDetails)
{
  size_t index = d->indexOf(stepDetails);
  if(index<d->cachedState.size())
  {
    StepInput rollingInput;
    d->execute(index, rollingInput);
    return d->cachedState.at(index).results;
  }
  return std::make_shared<tp_data::Collection>();
}

//##################################################################################################
StepInput PipelineManager::stepInput(StepDetails* stepDetails)
{
  StepInput rollingInput;
  size_t index = d->indexOf(stepDetails);
  index--;
    if(index<d->cachedState.size())
      d->execute(index, rollingInput);
  return rollingInput;
}

//##################################################################################################
StepInput PipelineManager::nextStepInput(StepDetails* stepDetails)
{
  StepInput rollingInput;
  size_t index = d->indexOf(stepDetails);
    if(index<d->cachedState.size())
      d->execute(index, rollingInput);
  return rollingInput;
}

//##################################################################################################
void PipelineManager::generateNextStepInput(std::vector<std::string>& errors,
                                            StepInput& result,
                                            const std::shared_ptr<tp_data::Collection>& input,
                                            StepDetails* stepDetails)
{
  {
    std::string error;
    d->updateRoot(error, input);
    if(!error.empty())
    {
      errors.push_back(error);
      return;
    }
  }

  size_t lastIndex = d->indexOf(stepDetails);
  errors = d->execute(lastIndex, result);
}

//##################################################################################################
PipelineDetails* PipelineManager::pipelineDetails()
{
  return d->pipelineDetails;
}

//##################################################################################################
std::vector<std::shared_ptr<tp_data::Collection>> PipelineManager::cachedState() const
{
  std::vector<std::shared_ptr<tp_data::Collection>> results;
  for(const auto& i : d->cachedState)
    if(i.results)
      results.push_back(i.results);
  return results;
}

//##################################################################################################
void PipelineManager::resetPersistentData()
{
  for(auto& i : d->cachedState)
    i.persistentData.reset();
}

}
