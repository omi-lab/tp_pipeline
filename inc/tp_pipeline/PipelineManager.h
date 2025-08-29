#pragma once

#include "tp_pipeline/Globals.h"
#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

namespace tp_data
{
class Collection;
class CollectionFactory;
}

namespace tp_pipeline
{
class PipelineManager;
class StepDelegate;


//##################################################################################################
//! This uses delegates to manage and execute a pipeline.
class PipelineManager
{
  TP_DQ;
public:
  //################################################################################################
  PipelineManager(PipelineDetails* pipelineDetails,
                  const StepDelegateMap* stepDelegates,
                  const tp_data::CollectionFactory* collectionFactory,
                  bool fixupParameters=true);

  //################################################################################################
  ~PipelineManager();

  //################################################################################################
  PipelineDetails* pipelineDetails() const;

  //################################################################################################
  void startExecution(StepDetails* finalStep=nullptr);

  //################################################################################################
  StepContext* takeNextAvailableStep();

  //################################################################################################
  void returnCompletedStep(StepContext* stepContext);

  //################################################################################################
  StepContext* stepContext(StepDetails* stepDetails) const;
};

}
