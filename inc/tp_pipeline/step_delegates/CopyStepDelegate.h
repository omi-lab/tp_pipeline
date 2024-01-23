#ifndef tp_pipeline_CopyStepDelegate_h
#define tp_pipeline_CopyStepDelegate_h

#include "tp_pipeline/AbstractStepDelegate.h"

namespace tp_pipeline
{

//##################################################################################################
class CopyStepDelegate: public tp_pipeline::AbstractStepDelegate
{
public:
  //################################################################################################
  CopyStepDelegate(const tp_data::CollectionFactory* collectionFactory);

  //################################################################################################
  void executeStep(tp_pipeline::StepDetails* stepDetails,
                   const tp_pipeline::StepInput& input,
                   tp_data::Collection& output,
                   tp_data::Collection& persistentData) const override;

  //################################################################################################
  void fixupParameters(tp_pipeline::StepDetails* stepDetails) const override;


  const tp_data::CollectionFactory* m_collectionFactory;
};

}

#endif
