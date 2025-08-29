#pragma once

#include "tp_pipeline/Globals.h"
#include "tp_pipeline/Parameter.h"
#include "tp_pipeline/ComplexObjectManager.h"

#include "tp_utils/RefCount.h"

#include <unordered_map>

namespace tp_pipeline
{
class PipelineDetails;

//##################################################################################################
struct TP_PIPELINE_SHARED_EXPORT PortMapping
{
  tp_utils::StringID portType;
  tp_utils::StringID portName; //!< The step delegate give each output a name (unique within that delegate)
  tp_utils::StringID dataName; //!< In the output collection we assign that data a unique name.

  //################################################################################################
  void saveState(nlohmann::json& j) const;

  //################################################################################################
  void loadState(const nlohmann::json& j);
};

//##################################################################################################
//! The configuration for a single step in an image processing pipeline
/*!
An image processing pipeline is made up of multiple steps, the initial input is passed into the
first step then the output of each step is passed into the next step, finally the output of the last
step is returned as the result of the pipeline.

The StepDetails hold the configuration for one step, this includes the name of the delegate that
will be used to execute the step.
 */
class TP_PIPELINE_SHARED_EXPORT StepDetails
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
  const std::pair<double, double>& position() const;

  //################################################################################################
  void setPosition(const std::pair<double, double>& position);

  //################################################################################################
  //! Returns the pipeline that this step is part of or nullptr.
  PipelineDetails* parent()const;

  //################################################################################################
  //! Saves the state of a step
  void saveBinary(nlohmann::json& j, const std::function<uint64_t(const std::string&)>& addBlob)noexcept;

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
  //! Sets the mapping between a port on the step delegate and the name of the data in the collection.
  void setInputMapping(const std::vector<PortMapping>& inputMapping);

  //################################################################################################
  const std::vector<PortMapping>& inputMapping() const;

  //################################################################################################
  //! Sets the mapping between a port on the step delegate and the name of the data in the collection.
  void setOutputMapping(const std::vector<PortMapping>& outputMapping);

  //################################################################################################
  const std::vector<PortMapping>& outputMapping() const;

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
