#include "tp_pipeline/StepDelegateMap.h"
#include "tp_pipeline/AbstractStepDelegate.h"

namespace tp_pipeline
{

//##################################################################################################
StepDelegateMap::StepDelegateMap()
{

}

//##################################################################################################
StepDelegateMap::~StepDelegateMap()
{
  for(const auto& i : m_stepDelegates)
    delete i.second;
}

//##################################################################################################
void StepDelegateMap::addStepDelegate(tp_pipeline::AbstractStepDelegate* stepDelegate)
{
  auto i = m_stepDelegates.find(stepDelegate->name());
  if(i != m_stepDelegates.end())
  {
    delete i->second;
    m_stepDelegates.erase(i);
  }
  m_stepDelegates[stepDelegate->name()] = stepDelegate;
}

//##################################################################################################
const tp_pipeline::AbstractStepDelegate* StepDelegateMap::stepDelegate(const tp_utils::StringID& name) const
{
  return tpGetMapValue(m_stepDelegates, name, nullptr);
}

//##################################################################################################
std::vector<tp_utils::StringID> StepDelegateMap::stepDelegateNames() const
{
  std::vector<tp_utils::StringID> stepDelegateNames;
  for(const auto& i : m_stepDelegates)
    stepDelegateNames.push_back(i.first);
  return stepDelegateNames;
}

}
