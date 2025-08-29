#pragma once

#include "tp_pipeline/StepDelegate.h"

namespace tp_pipeline
{

//##################################################################################################
class NoneStepDelegate: public tp_pipeline::StepDelegate
{
public:
  //################################################################################################
  NoneStepDelegate();

  //################################################################################################
  void executeStep(StepContext* stepContext) const override;

  //################################################################################################
  void fixupParameters(tp_pipeline::StepDetails* stepDetails) const override;
};

}
