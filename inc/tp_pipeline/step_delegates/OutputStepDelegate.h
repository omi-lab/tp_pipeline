#pragma once

#include "tp_pipeline/Globals.h"

#include "tp_pipeline/StepDelegate.h"

namespace tp_pipeline
{

//##################################################################################################
class OutputStepDelegate: public StepDelegate
{
public:
  //################################################################################################
  OutputStepDelegate(const tp_utils::StringID& name, const std::vector<PortDetails>& inPorts);

  //################################################################################################
  void fixupParameters(StepDetails* stepDetails) const override;

  //################################################################################################
  std::string fileDirectory(StepDetails* stepDetails) const;
};

}
