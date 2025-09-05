#include "tp_pipeline/StepContext.h"
#include "tp_pipeline/StepDetails.h"

#include "tp_data/Collection.h"

namespace tp_pipeline
{

//##################################################################################################
bool StepContext::dependenciesMet() const
{
  for(auto s : dependsOn)
    if(!s->runComplete)
      return false;

  return true;
}

//##################################################################################################
const tp_utils::StringID&  StepContext::inputDataNameFromPortName(const tp_utils::StringID& portName) const
{
  static const tp_utils::StringID n;
  for(const auto& m : stepDetails->inputMapping())
    if(m.portName == portName)
      return m.dataName;
  return n;
}

}
