#include "tp_pipeline/step_delegates/CopyStepDelegate.h"
#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"
#include "tp_data/AbstractMember.h"
#include "tp_data/CollectionFactory.h"

namespace tp_pipeline
{

//##################################################################################################
CopyStepDelegate::CopyStepDelegate(const tp_data::CollectionFactory* collectionFactory):
  AbstractStepDelegate(copySID(), {}),
  m_collectionFactory(collectionFactory)
{

}

//##################################################################################################
void CopyStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                   const tp_pipeline::StepInput& inputs,
                                   tp_data::Collection& output,
                                   tp_data::Collection& persistentData) const
{
  TP_UNUSED(persistentData);

  Parameter memberNamesParameter = stepDetails->parameter(memberNamesSID());

  std::vector<std::string> memberNames = tpGetVariantValue<std::vector<std::string>>(memberNamesParameter.value);

  for(const auto& memberName : memberNames)
  {
    auto member = inputs.member(memberName);

    if(!member)
    {
      output.addError("Copy failed to find: " + memberName);
      continue;
    }

    std::string error;
    auto newMember = m_collectionFactory->clone(error, *member);
    if(!newMember)
    {
      output.addError("Copy failed to clone: " + memberName);
      output.addError(error);
      continue;
    }

    newMember->setName(memberName);
    output.addMember(newMember);
  }
}

//##################################################################################################
void CopyStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  std::vector<tp_utils::StringID> validParams{memberNamesSID()};
  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}
