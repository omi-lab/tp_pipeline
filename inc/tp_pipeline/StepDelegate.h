#pragma once

#include "tp_pipeline/Globals.h"

namespace tp_data
{
class Collection;
}

namespace tp_pipeline
{
struct StepContext;
class StepDetails;

//##################################################################################################
struct PortDetails
{
  tp_utils::StringID name;
  tp_utils::StringID type;
};

//##################################################################################################
class TP_PIPELINE_SHARED_EXPORT StepDelegate
{
  TP_DQ;
public:
  //################################################################################################
  StepDelegate(const tp_utils::StringID& name,
               const std::vector<tp_utils::StringID>& groups,
               const std::vector<PortDetails>& inPorts,
               const std::vector<PortDetails>& outPorts);

  //################################################################################################
  virtual ~StepDelegate();

  //################################################################################################
  const tp_utils::StringID& name() const;

  //################################################################################################
  const std::vector<tp_utils::StringID>& groups() const;

  //################################################################################################
  const std::vector<PortDetails>& inPorts() const;

  //################################################################################################
  const std::vector<PortDetails>& outPorts() const;

  //################################################################################################
  //! Execute this step in the pipeline.
  /*!
  This method should be reimplemented to perform the work of the step. The stepDetails contains the
  configuration for the step, the input and output are for the data passing through the step.

  \param stepDetails contins the configuration for the step.
  \param input data read from here.
  \param output data should be written to here.
  */
  virtual void executeStep(StepContext* stepContext) const=0;

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
