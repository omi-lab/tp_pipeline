#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/PipelineDetails.h"

#include "tp_utils/JSONUtils.h"

namespace tp_pipeline
{

//##################################################################################################
struct StepDetails::Private
{
  StepDetails* q;
  PipelineDetails* parent;
  tp_utils::StringID delegateName;

  std::unordered_map<tp_utils::StringID, Parameter> parameters;
  std::vector<tp_utils::StringID> parametersOrder;

  std::vector<std::string> outputNames;
  std::vector<std::pair<std::string, std::string>> outputMapping;


  //################################################################################################
  Private(StepDetails* q_, const tp_utils::StringID& delegateName_):
    q(q_),
    parent(nullptr),
    delegateName(delegateName_)
  {

  }
};

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
  d->outputNames        = other.d->outputNames;
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
PipelineDetails* StepDetails::parent()const
{
  return d->parent;
}

//##################################################################################################
nlohmann::json StepDetails::saveBinary(const std::function<uint64_t(const std::string&)>& addBlob) noexcept
{
  nlohmann::json j;

  j["Delegate"] = d->delegateName.toString();

  j["Parameters"] = nlohmann::json::array();
  for(const auto& i : d->parameters)
  {
    j["Parameters"].push_back(i.second.saveBinary(addBlob));
  }

  j["Parameters order"] = nlohmann::json::array();
  for(const auto& i : d->parametersOrder)
    j["Parameters order"].push_back(i.toString());

  j["Complex objects"] = complexObjectManager.saveBinary(addBlob);

  j["Output mapping"] = nlohmann::json::array();
  for(const auto& i : d->outputMapping)
    j["Output mapping"].push_back({{"src", i.first}, {"dst", i.second}});

  return j;
}

//##################################################################################################
void StepDetails::loadBinary(const nlohmann::json& j, const std::vector<std::string>& blobs) noexcept
{
  d->delegateName = TPJSONString(j, "Delegate", "None");

  d->parameters.clear();
  if(auto i = j.find("Parameters"); i != j.end() && i->is_array())
  {
    for(const auto& jj : *i)
    {
      Parameter parameter(jj, blobs);
      if(parameter.name.isValid())
        d->parameters[parameter.name] = parameter;
    }
  }

  d->parametersOrder.clear();
  for(const tp_utils::StringID& name : tp_utils::getJSONStringIDs(j, "Parameters order"))
    if(name.isValid())
      d->parametersOrder.push_back(name);

  if(auto i=j.find("Complex objects"); i!=j.end())
    complexObjectManager.loadBinary(*i, blobs);
  else
    complexObjectManager.clearComplexObjects();

  d->outputMapping.clear();
  if(auto i = j.find("Output mapping"); i != j.end() && i->is_array())
  {
    for(const auto& m: *i)
    {
      std::string src = m.value("src", std::string());
      std::string dst = m.value("dst", std::string());
      if(!src.empty() && !dst.empty())
        d->outputMapping.push_back({src, dst});
    }
  }

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
void StepDetails::setOutputMapping(const std::vector<std::pair<std::string, std::string> >& outputMapping)
{
  d->outputMapping = outputMapping;
  invalidate();
}

//##################################################################################################
const std::vector<std::pair<std::string, std::string>>& StepDetails::outputMapping() const
{
  return d->outputMapping;
}

//##################################################################################################
const std::vector<std::string>& StepDetails::outputNames() const
{
  return d->outputNames;
}

//##################################################################################################
void StepDetails::setOutputNames(const std::vector<std::string>& outputNames)
{
  d->outputNames = outputNames;
  invalidate();
}

//##################################################################################################
std::string StepDetails::lookupOutputName(const std::string& outputName) const
{
  for(const auto& [from, to] : d->outputMapping)
    if(from == outputName)
      return to;

  return outputName;
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
