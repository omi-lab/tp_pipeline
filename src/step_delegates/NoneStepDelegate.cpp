#include "tp_pipeline/step_delegates/NoneStepDelegate.h"
#include "tp_pipeline/StepDetails.h"

namespace tp_pipeline
{

//##################################################################################################
NoneStepDelegate::NoneStepDelegate():
  StepDelegate(noneSID(), {}, {}, {})
{

}

//##################################################################################################
void NoneStepDelegate::executeStep(StepContext* stepContext) const
{
  TP_UNUSED(stepContext);
}

//##################################################################################################
void NoneStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  std::vector<tp_utils::StringID> validParams;
  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
