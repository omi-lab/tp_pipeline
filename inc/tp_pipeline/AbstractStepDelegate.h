#ifndef tp_pipeline_AbstractStepDelegate_h
#define tp_pipeline_AbstractStepDelegate_h

#include "tp_pipeline/Globals.h"

#include <memory>

namespace tp_data
{
class Collection;
}

namespace tp_pipeline
{
struct StepInput;
class StepDetails;

//##################################################################################################
class AbstractStepDelegate
{
  TP_DQ;
public:
  //################################################################################################
  AbstractStepDelegate(const tp_utils::StringID& name, const std::vector<tp_utils::StringID>& groups);

  //################################################################################################
  virtual ~AbstractStepDelegate();

  //################################################################################################
  const tp_utils::StringID& name()const;

  //################################################################################################
  const std::vector<tp_utils::StringID>& groups() const;

  //################################################################################################
  //! Execute this step in the pipeline.
  /*!
  This method should be reimplemented to perform the work of the step. The stepDetails contains the
  configuration for the step, the input and output are for the data passing through the step, and
  the persistentData is for steps that need to persist data between frames.

  For pipelines operating on video data it is often necessary to access data from previous frames,
  for example to calculate optical flow you need access to the currrent and previous frames. To
  enable this the persistentData collection is provided, this will be passed into this step for each
  pass of the pipeline. Each step has its own persistentData collection.

  \param stepDetails contins the configuration for the step.
  \param input data read from here.
  \param output data should be written to here.
  \param persistentData belonging to this step.
  */
  virtual void executeStep(StepDetails* stepDetails,
                           const StepInput& input,
                           tp_data::Collection& output,
                           tp_data::Collection& persistentData) const=0;

  //################################################################################################
  //! This gets called to add, adapt, and validate step parametes
  /*!
  This should add any parameters to the step that are missing, remove any that are not needed, and
  alter any that are not valid. This will be called on the blan step to add the default set of
  parameters, it will then be called each time the step is altered to make sure that it has a valid
  set of parameters.
  \param stepDetails
  */
  virtual void fixupParameters(StepDetails* stepDetails) const=0;
};

}

#endif
