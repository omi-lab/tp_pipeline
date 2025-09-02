#pragma once

#include "tp_pipeline/Globals.h"

#include "tp_pipeline/StepDelegate.h"

namespace tp_pipeline
{

//##################################################################################################
class InputStepDelegate: public StepDelegate
{
public:
  //################################################################################################
  InputStepDelegate(const tp_utils::StringID& name, const std::vector<PortDetails>& outPorts);

  //################################################################################################
  void fixupParameters(StepDetails* stepDetails) const override;

  //################################################################################################
  virtual size_t nFiles(StepDetails* stepDetails) const = 0;

  //################################################################################################
  std::string fileDirectory(StepDetails* stepDetails) const;

  //################################################################################################
  size_t fileIndex(StepDetails* stepDetails) const;
};

}
