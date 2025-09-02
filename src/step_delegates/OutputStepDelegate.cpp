#include "tp_pipeline/step_delegates/OutputStepDelegate.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepContext.h"

namespace tp_pipeline
{
namespace
{

//##################################################################################################
void _fixupParameters(StepDetails* stepDetails)
{
  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();
  {
    const tp_utils::StringID& name = fileIndexSID();
    auto param = tpGetMapValue(parameters, name);
  }

  std::string directory;

  {
    const tp_utils::StringID& name = fileDirectorySID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The directory to save files to.";
    param.type = directorySID();
    directory = tpGetVariantValue<std::string>(param.value);
    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}
}

#define GROUPS    {outputsSID()}
#define OUT_PORTS  {}

//##################################################################################################
OutputStepDelegate::OutputStepDelegate(const tp_utils::StringID& name, const std::vector<PortDetails>& inPorts):
  StepDelegate(name, GROUPS, inPorts, OUT_PORTS)
{

}

//##################################################################################################
void OutputStepDelegate::fixupParameters(StepDetails* stepDetails) const
{
  _fixupParameters(stepDetails);
}

//##################################################################################################
std::string OutputStepDelegate::fileDirectory(StepDetails* stepDetails) const
{
  return stepDetails->parameterValue<std::string>(fileDirectorySID());
}

}
