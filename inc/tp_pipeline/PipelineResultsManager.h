#pragma once

#include "tp_pipeline/Globals.h"
#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepContext.h"

#include "tp_utils/CallbackCollection.h"

namespace tp_data
{
class Collection;
}

namespace tp_pipeline
{
class PipelineManager;

//##################################################################################################
//! Keeps a copy of data from an execution run of a pipeline.
class TP_PIPELINE_SHARED_EXPORT PipelineResultsManager
{
  TP_DQ;
public:
  //################################################################################################
  PipelineResultsManager(PipelineDetails* pipelineDetails);

  //################################################################################################
  ~PipelineResultsManager();

  //################################################################################################
  PipelineDetails* pipelineDetails() const;

  //################################################################################################
  void copyResultsFromManager(const PipelineManager& pipelineManager);

  //################################################################################################
  std::shared_ptr<tp_data::Collection> stepInput(StepDetails* stepDetails) const;

  //################################################################################################
  std::shared_ptr<tp_data::Collection> stepOutput(StepDetails* stepDetails) const;

  //################################################################################################
  void stepInputOutput(StepDetails* stepDetails,
                       std::shared_ptr<tp_data::Collection>& stepInput,
                       std::shared_ptr<tp_data::Collection>& stepOutput) const;

  //################################################################################################
  tp_utils::CallbackCollection<void()> changed;
};

}
