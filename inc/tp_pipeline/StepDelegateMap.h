#ifndef tp_pipeline_StepDelegateMap_h
#define tp_pipeline_StepDelegateMap_h

#include "tp_pipeline/Globals.h"

namespace tp_pipeline
{
class AbstractStepDelegate;

//##################################################################################################
class StepDelegateMap
{
public:
  //################################################################################################
  StepDelegateMap();

  //################################################################################################
  ~StepDelegateMap();

  //################################################################################################
  void addStepDelegate(tp_pipeline::AbstractStepDelegate* stepDelegate);

  //################################################################################################
  const tp_pipeline::AbstractStepDelegate* stepDelegate(const tp_utils::StringID& name) const;

  //################################################################################################
  std::vector<tp_utils::StringID> stepDelegateNames() const;

private:
  std::unordered_map<tp_utils::StringID, tp_pipeline::AbstractStepDelegate*> m_stepDelegates;
};

}

#endif
