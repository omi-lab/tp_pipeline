#include "tp_pipeline/step_delegates/CopyStepDelegate.h"
#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepOutput.h"

#include "tp_data/CollectionFactory.h"

namespace tp_pipeline
{

//##################################################################################################
CopyStepDelegate::CopyStepDelegate(const tp_data::CollectionFactory* collectionFactory):
  StepDelegate(copySID(), {}, {}, {}),
  m_collectionFactory(collectionFactory)
{

}

//##################################################################################################
void CopyStepDelegate::executeStep(StepContext* stepContext) const
{
  TP_UNUSED(stepContext);
  // Parameter memberNamesParameter = stepDetails->parameter(memberNamesSID());

  // std::vector<std::string> memberNames = tpGetVariantValue<std::vector<std::string>>(memberNamesParameter.value);

  // for(const auto& memberName : memberNames)
  // {
  //   auto member = input.member(memberName);

  //   if(!member)
  //   {
  //     stepContext->stepOutput->addError("Copy failed to find: " + memberName);
  //     continue;
  //   }

  //   std::string error;
  //   auto newMember = m_collectionFactory->clone(error, *member);
  //   if(!newMember)
  //   {
  //     stepContext->stepOutput->addError("Copy failed to clone: " + memberName);
  //     stepContext->stepOutput->addError(error);
  //     continue;
  //   }

  //   newMember->setName(memberName);
  //   stepContext->stepOutput->addMember(newMember);
  // }
}

//##################################################################################################
void CopyStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  std::vector<tp_utils::StringID> validParams{memberNamesSID()};
  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
