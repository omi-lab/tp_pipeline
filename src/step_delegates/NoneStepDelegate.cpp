#include "tp_pipeline/step_delegates/NoneStepDelegate.h"
#include "tp_pipeline/StepDetails.h"

namespace tp_pipeline
{

//##################################################################################################
NoneStepDelegate::NoneStepDelegate():
  AbstractStepDelegate(noneSID(), {})
{

}

//##################################################################################################
void NoneStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                   const tp_pipeline::StepInput& inputs,
                                   tp_data::Collection& output,
                                   tp_data::Collection& persistentData) const
{
  TP_UNUSED(stepDetails);
  TP_UNUSED(inputs);
  TP_UNUSED(output);
  TP_UNUSED(persistentData);
}

//##################################################################################################
void NoneStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  std::vector<tp_utils::StringID> validParams;
  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
