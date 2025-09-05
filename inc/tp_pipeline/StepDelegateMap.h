#pragma once

#include "tp_pipeline/Globals.h"

#include "tp_utils/TPPixel.h"

#include <unordered_map>

namespace tp_pipeline
{
class StepDelegate;

//##################################################################################################
struct TP_PIPELINE_SHARED_EXPORT DataTypeProperties
{
  tp_utils::StringID type;
  TPPixel color; //!< Color used to draw the data in node graphs.
};

//##################################################################################################
class TP_PIPELINE_SHARED_EXPORT StepDelegateMap
{
public:
  //################################################################################################
  StepDelegateMap();

  //################################################################################################
  ~StepDelegateMap();

  //################################################################################################
  void addStepDelegate(tp_pipeline::StepDelegate* stepDelegate);

  //################################################################################################
  const tp_pipeline::StepDelegate* stepDelegate(const tp_utils::StringID& name) const;

  //################################################################################################
  std::vector<tp_utils::StringID> stepDelegateNames() const;

private:
  std::unordered_map<tp_utils::StringID, tp_pipeline::StepDelegate*> m_stepDelegates;
};

}
