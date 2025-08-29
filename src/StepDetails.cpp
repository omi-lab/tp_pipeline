#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/PipelineDetails.h"

#include "tp_utils/JSONUtils.h"

namespace tp_pipeline
{

//##################################################################################################
struct StepDetails::Private
{
  Q* q;
  PipelineDetails* parent;
  tp_utils::StringID delegateName;

  std::unordered_map<tp_utils::StringID, Parameter> parameters;
  std::vector<tp_utils::StringID> parametersOrder;

  std::vector<PortMapping> inputMapping;
  std::vector<PortMapping> outputMapping;

  std::pair<double, double> position{0.0, 0.0};


  //################################################################################################
  Private(Q* q_, const tp_utils::StringID& delegateName_):
    q(q_),
    parent(nullptr),
    delegateName(delegateName_)
  {

  }
};

//##################################################################################################
void PortMapping::saveState(nlohmann::json& j) const
{
  j["portType"] = portType.toString();
  j["portName"] = portName.toString();
  j["dataName"] = dataName.toString();
}

//##################################################################################################
void PortMapping::loadState(const nlohmann::json& j)
{
  portType = TPJSONString(j, "portType");
  portName = TPJSONString(j, "portName");
  dataName = TPJSONString(j, "dataName");
}

//##################################################################################################
StepDetails::StepDetails(const tp_utils::StringID& delegateName):
  d(new Private(this, delegateName))
{

}

//##################################################################################################
StepDetails::StepDetails(const StepDetails& other):
  d(new Private(this, other.d->delegateName)),
  complexObjectManager(other.complexObjectManager)
{
  d->parameters         = other.d->parameters;
  d->parametersOrder    = other.d->parametersOrder;
  d->inputMapping      = other.d->inputMapping;
  d->outputMapping      = other.d->outputMapping;
}

//##################################################################################################
void StepDetails::setDelegateName(const tp_utils::StringID& delegateName)
{
  d->delegateName = delegateName;
  complexObjectManager.clearComplexObjects();
  d->parameters.clear();
  d->parametersOrder.clear();
  invalidate();
}

//##################################################################################################
const tp_utils::StringID& StepDetails::delegateName()const
{
  return d->delegateName;
}

//##################################################################################################
const std::pair<double, double>& StepDetails::position() const
{
  return d->position;
}

//##################################################################################################
void StepDetails::setPosition(const std::pair<double, double>& position)
{
  if(d->position == position)
    return;

  d->position = position;
  invalidate();
}

//##################################################################################################
PipelineDetails* StepDetails::parent()const
{
  return d->parent;
}

//##################################################################################################
void StepDetails::saveBinary(nlohmann::json& j, const std::function<uint64_t(const std::string&)>& addBlob) noexcept
{
  j["delegateName"] = d->delegateName.toString();

  {
    auto& jj = j["parameters"];
    jj = nlohmann::json::array();
    jj.get_ptr<nlohmann::json::array_t*>()->reserve(d->parameters.size());
    for(const auto& i : d->parameters)
    {
      jj.emplace_back();
      i.second.saveBinary(jj.back(), addBlob);
    }
  }

  tp_utils::saveVectorOfStringIDsToJSON(j["parametersOrder"], d->parametersOrder);

  complexObjectManager.saveBinary(j["complexObjectManager"], addBlob);

  tp_utils::saveVectorOfObjectsToJSON(j["inputMapping"], d->inputMapping);
  tp_utils::saveVectorOfObjectsToJSON(j["outputMapping"], d->outputMapping);

  j["position"] = nlohmann::json::array({d->position.first, d->position.second});
}

//##################################################################################################
void StepDetails::loadBinary(const nlohmann::json& j, const std::vector<std::string>& blobs) noexcept
{
  d->delegateName = TPJSONString(j, "delegateName", "None");

  d->parameters.clear();
  if(auto i = j.find("parameters"); i != j.end() && i->is_array())
  {
    for(const auto& jj : *i)
    {
      Parameter parameter(jj, blobs);
      if(parameter.name.isValid())
        d->parameters[parameter.name] = parameter;
    }
  }

  tp_utils::loadVectorOfStringIDsFromJSON(j, "parametersOrder", d->parametersOrder);

  if(auto i=j.find("complexObjectManager"); i!=j.end())
    complexObjectManager.loadBinary(*i, blobs);
  else
    complexObjectManager.clearComplexObjects();

  tp_utils::loadVectorOfObjectsFromJSON(j, "inputMapping", d->inputMapping);
  tp_utils::loadVectorOfObjectsFromJSON(j, "outputMapping", d->outputMapping);

  for(auto& m : d->outputMapping)
    if(!m.dataName.isValid())
      m.dataName = randomId();

  if(auto i = j.find("position");
     i != j.end() &&
     i->is_array() &&
     i->size() == 2 &&
     i->at(0).is_number()&&
     i->at(1).is_number())
    d->position = {i->at(0).get<double>(), i->at(1).get<double>()};
  else
    d->position = {0.0, 0.0};

  invalidate();
}

//##################################################################################################
const std::unordered_map<tp_utils::StringID, Parameter>& StepDetails::parameters()
{
  return d->parameters;
}

//##################################################################################################
void StepDetails::setParamerter(const Parameter& parameter)
{
  d->parameters[parameter.name] = parameter;
  invalidate();
}

//##################################################################################################
void StepDetails::setParameterValue(const tp_utils::StringID& name, const Variant& value)
{
  d->parameters[name].value = value;
  invalidate();
}

//##################################################################################################
void StepDetails::removeParameter(const tp_utils::StringID& name)
{
  auto i = d->parameters.find(name);
  if(i!=d->parameters.end())
    d->parameters.erase(i);
  invalidate();
}

//##################################################################################################
const std::vector<tp_utils::StringID>& StepDetails::parametersOrder()const
{
  return d->parametersOrder;
}

//##################################################################################################
void StepDetails::setParametersOrder(const std::vector<tp_utils::StringID>& parametersOrder)
{
  d->parametersOrder = parametersOrder;
  invalidate();
}

//##################################################################################################
void StepDetails::setValidParameters(const std::vector<tp_utils::StringID>& validParams)
{
  for (auto i = d->parameters.begin(); i != d->parameters.end();)
  {
    if(!tpContains(validParams, i->first))
      i = d->parameters.erase(i);
    else
      ++i;
  }
}

//##################################################################################################
void StepDetails::setInputMapping(const std::vector<PortMapping>& inputMapping)
{
  d->inputMapping = inputMapping;
  invalidate();
}

//##################################################################################################
const std::vector<PortMapping>& StepDetails::inputMapping() const
{
  return d->inputMapping;
}

//##################################################################################################
void StepDetails::setOutputMapping(const std::vector<PortMapping>& outputMapping)
{
  d->outputMapping = outputMapping;
  invalidate();
}

//##################################################################################################
const std::vector<PortMapping>& StepDetails::outputMapping() const
{
  return d->outputMapping;
}

//##################################################################################################
void StepDetails::invalidate()
{
  if(d->parent)
    d->parent->invalidateStep(this);
}

//##################################################################################################
StepDetails::~StepDetails()
{
  delete d;
}

//##################################################################################################
void StepDetails::setParent(PipelineDetails* parent)
{
  d->parent = parent;
}

}
