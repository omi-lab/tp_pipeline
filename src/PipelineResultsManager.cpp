#include "tp_pipeline/PipelineResultsManager.h"
#include "tp_pipeline/PipelineManager.h"
#include "tp_pipeline/PipelineDetails.h"

namespace tp_pipeline
{

namespace
{
//##################################################################################################
struct StepDetails_lt
{
  std::shared_ptr<tp_data::Collection> stepInput;
  std::shared_ptr<tp_data::Collection> stepOutput;
};
}

//##################################################################################################
struct PipelineResultsManager::Private
{
  Q* q;
  PipelineDetails* pipelineDetails;

  std::unordered_map<StepDetails*, StepDetails_lt> savedState;

  //################################################################################################
  Private(Q* q_, PipelineDetails* pipelineDetails_):
    q(q_),
    pipelineDetails(pipelineDetails_)
  {
    deletedCallback.connect(pipelineDetails->deletedCallback);
  }

  //################################################################################################
  tp_utils::Callback<void(StepDetails*)> deletedCallback = [&](StepDetails* stepDetails)
  {
    savedState.erase(stepDetails);
    q->changed();
  };
};

//##################################################################################################
PipelineResultsManager::PipelineResultsManager(PipelineDetails* pipelineDetails):
  d(new Private(this, pipelineDetails))
{

}

//##################################################################################################
PipelineResultsManager::~PipelineResultsManager()
{
  delete d;
}

//##################################################################################################
PipelineDetails* PipelineResultsManager::pipelineDetails() const
{
  return d->pipelineDetails;
}

//##################################################################################################
void PipelineResultsManager::copyResultsFromManager(const PipelineManager& pipelineManager)
{
  d->savedState.clear();
  const auto& contexts = pipelineManager.stepContexts();
  d->savedState.reserve(contexts.size());

  for(const auto& context : contexts)
  {
    auto& savedState = d->savedState[context.stepDetails];

    savedState.stepInput = context.stepInput;
    savedState.stepOutput = context.stepOutput->output();
  }

  changed();
}

//##################################################################################################
std::shared_ptr<tp_data::Collection> PipelineResultsManager::stepInput(StepDetails* stepDetails) const
{
  if(auto i=d->savedState.find(stepDetails); i!=d->savedState.end())
    return i->second.stepInput;
  return {};
}

//##################################################################################################
std::shared_ptr<tp_data::Collection> PipelineResultsManager::stepOutput(StepDetails* stepDetails) const
{
  if(auto i=d->savedState.find(stepDetails); i!=d->savedState.end())
    return i->second.stepOutput;
  return {};
}



//##################################################################################################
void PipelineResultsManager::stepInputOutput(StepDetails* stepDetails,
                                             std::shared_ptr<tp_data::Collection>& stepInput,
                                             std::shared_ptr<tp_data::Collection>& stepOutput) const
{
  if(auto i=d->savedState.find(stepDetails); i!=d->savedState.end())
  {
    stepInput = i->second.stepInput;
    stepOutput = i->second.stepOutput;
  }
}

}
