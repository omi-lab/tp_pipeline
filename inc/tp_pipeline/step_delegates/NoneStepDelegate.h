#ifndef tp_pipeline_NoneStepDelegate_h
#define tp_pipeline_NoneStepDelegate_h

#include "tp_pipeline/AbstractStepDelegate.h"

namespace tp_pipeline
{

//##################################################################################################
class NoneStepDelegate: public tp_pipeline::AbstractStepDelegate
{
public:
  //################################################################################################
  NoneStepDelegate();

  //################################################################################################
  void executeStep(tp_pipeline::StepDetails* stepDetails,
                   const tp_pipeline::StepInput& input,
                   tp_data::Collection& output,
                   tp_data::Collection& persistentData) const override;

  //################################################################################################
  void fixupParameters(tp_pipeline::StepDetails* stepDetails) const override;
};

}

#endif
