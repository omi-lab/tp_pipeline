#include "tp_pipeline/step_delegates/InputStepDelegate.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepContext.h"

namespace tp_pipeline
{
namespace
{

//##################################################################################################
void _fixupParameters(const InputStepDelegate* stepDelegate, StepDetails* stepDetails)
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
    param.description = "The directory to load files from.";
    param.type = directorySID();
    directory = tpGetVariantValue<std::string>(param.value);
    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    const tp_utils::StringID& name = fileIndexSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The index of the file in the directory.";
    param.type = sizeSID();
    param.min = 0;
    param.max = 0;

    if(!directory.empty())
      param.max = tpMax(size_t(1), stepDelegate->nFiles(stepDetails))-1;

    param.validateBounds<size_t>();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}
}

#define GROUPS    {inputsSID()}
#define IN_PORTS  {}

//##################################################################################################
InputStepDelegate::InputStepDelegate(const tp_utils::StringID& name, const std::vector<PortDetails>& outPorts):
  StepDelegate(name, GROUPS, IN_PORTS, outPorts)
{

}

//##################################################################################################
void InputStepDelegate::fixupParameters(StepDetails* stepDetails) const
{
  _fixupParameters(this, stepDetails);
}

//##################################################################################################
std::string InputStepDelegate::fileDirectory(StepDetails* stepDetails) const
{
  return stepDetails->parameterValue<std::string>(fileDirectorySID());
}

//##################################################################################################
size_t InputStepDelegate::fileIndex(StepDetails* stepDetails) const
{
  return stepDetails->parameterValue<size_t>(fileIndexSID());
}

}
