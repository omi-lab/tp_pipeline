#ifndef tp_pipeline_StepDetails_h
#define tp_pipeline_StepDetails_h

#include "tp_pipeline/Globals.h"
#include "tp_pipeline/Parameter.h"
#include "tp_pipeline/ComplexObjectManager.h"

#include "tp_utils/RefCount.h"

#include "json.hpp"

#include <unordered_map>

namespace tp_pipeline
{
class PipelineDetails;

//##################################################################################################
//! The configuration for a single step in an image processing pipeline
/*!
An image processing pipeline is made up of multiple steps, the initial input is passed into the
first step then the output of each step is passed into the next step, finally the output of the last
step is returned as the result of the pipeline.

The StepDetails hold the configuration for one step, this includes the name of the delegate that
will be used to execute the step.
 */
class StepDetails
{
  friend class PipelineDetails;
  TP_REF_COUNT_OBJECTS("StepDetails");
  TP_DQ;
  StepDetails& operator=(const StepDetails&) = delete;
public:
  //################################################################################################
  /*!
  \param delegateName - The name of the delegate to use for executing this step.
  */
  StepDetails(const tp_utils::StringID& delegateName=tp_utils::StringID());

  //################################################################################################
  StepDetails(const StepDetails& other);

  //################################################################################################
  //! Set the name of the delegate used to execute this step
  /*!
  This sets the name that will be used to select the delegate that will be used to execute this
  step.

  \param delegateName - The name of the delegate to use for executing this step.
  */
  void setDelegateName(const tp_utils::StringID& delegateName);

  //################################################################################################
  //! Returns the name of the delegate that will be used to execute the step
  const tp_utils::StringID& delegateName()const;

  //################################################################################################
  //! Returns the pipeline that this step is part of or nullptr.
  PipelineDetails* parent()const;

  //################################################################################################
  //! Saves the state of a step
  nlohmann::json saveBinary(const std::function<uint64_t(const std::string&)>& addBlob)noexcept;

  //################################################################################################
  //! loads the state of a step
  void loadBinary(const nlohmann::json& j, const std::vector<std::string>& blobs)noexcept;

  //################################################################################################
  const std::unordered_map<tp_utils::StringID, Parameter>& parameters();

  //################################################################################################
  void setParamerter(const Parameter& parameter);

  //################################################################################################
  void setParameterValue(const tp_utils::StringID& name, const Variant& value);

  //################################################################################################
  void removeParameter(const tp_utils::StringID& name);

  //################################################################################################
  Parameter parameter(const tp_utils::StringID& name)
  {
    const auto& p = parameters();
    const auto i = p.find(name);
    return (i == p.end())?Parameter():i->second;
  }

  //################################################################################################
  template<typename T>
  T parameterValue(const tp_utils::StringID& name, const T& defaultValue=T())
  {
    const auto& p = parameters();
    const auto i = p.find(name);
    if(i == p.end())
      return defaultValue;

    return tpGetVariantValue(i->second.value, defaultValue);
  }

  //################################################################################################
  const std::vector<tp_utils::StringID>& parametersOrder()const;

  //################################################################################################
  //! Uses this to set the order of parameters
  void setParametersOrder(const std::vector<tp_utils::StringID>& parametersOrder);

  //################################################################################################
  //! This will remove invalid parameters
  void setValidParameters(const std::vector<tp_utils::StringID>& validParams);

  //################################################################################################
  ComplexObjectManager complexObjectManager;

  //################################################################################################
  void setOutputMapping(const std::vector<std::pair<std::string, std::string>>& outputMapping);

  //################################################################################################
  const std::vector<std::pair<std::string, std::string>>& outputMapping() const;

  //################################################################################################
  const std::vector<std::string>& outputNames() const;

  //################################################################################################
  void setOutputNames(const std::vector<std::string>& outputNames);

  //################################################################################################
  std::string lookupOutputName(const std::string& outputName) const;

  //################################################################################################
  void invalidate();

  //################################################################################################
  //Called by the pipeline
  ~StepDetails();

private:
  //################################################################################################
  //Called by the pipeline
  void setParent(PipelineDetails* parent);
};

}

#endif
