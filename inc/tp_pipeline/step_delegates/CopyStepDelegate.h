#pragma once

#include "tp_pipeline/StepDelegate.h"

namespace tp_pipeline
{

//##################################################################################################
class CopyStepDelegate: public tp_pipeline::StepDelegate
{
public:
  //################################################################################################
  CopyStepDelegate(const tp_data::CollectionFactory* collectionFactory);

  //################################################################################################
  void executeStep(StepContext* stepContext) const override;

  //################################################################################################
  void fixupParameters(tp_pipeline::StepDetails* stepDetails) const override;


  const tp_data::CollectionFactory* m_collectionFactory;
};

}
