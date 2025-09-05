#pragma once

#include "tp_pipeline/Globals.h"

#include <memory>

namespace tp_data
{
class Collection;
class AbstractMember;
}

namespace tp_pipeline
{
class StepDelegate;

//##################################################################################################
class TP_PIPELINE_SHARED_EXPORT StepOutput
{
  TP_NONCOPYABLE(StepOutput);
  TP_DQ;
public:
  //################################################################################################
  StepOutput(const StepDelegate* stepDelegate, StepDetails* stepDetails);

  //################################################################################################
  ~StepOutput();

  //################################################################################################
  bool addMember(const tp_utils::StringID& portName, const std::shared_ptr<tp_data::AbstractMember>& member);

  //################################################################################################
  void addError(const std::string& error);

  //################################################################################################
  const std::shared_ptr<tp_data::Collection>& output() const;
};

}
