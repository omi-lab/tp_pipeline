#include "tp_pipeline/Parameter.h"

#include "tp_utils/JSONUtils.h"

namespace tp_pipeline
{

//##################################################################################################
Parameter::Parameter()
{

}

//##################################################################################################
Parameter::Parameter(const nlohmann::json& j)
{
  load(j);
}

//##################################################################################################
Parameter::Parameter(const nlohmann::json& j, const std::vector<std::string>& blobs)
{
  loadBinary(j, blobs);
}

//##################################################################################################
nlohmann::json Parameter::save()const
{
  nlohmann::json j;

  j["Name"]        = name.toString();
  j["Type"]        = type.toString();
  j["Value"]       = value           ;
  j["Min"]         = min             ;
  j["Max"]         = max             ;
  j["Step"]        = step            ;
  j["Mutate"]      = mutate          ;
  j["Description"] = description     ;

  return j;
}

//##################################################################################################
void Parameter::load(const nlohmann::json& j)
{
  name        = TPJSONString(j, "Name");
  type        = TPJSONString(j, "Type");
  mutate      = TPJSONBool  (j, "Mutate");
  description = TPJSONString(j, "Description");

  value = variantFromJSON(TPJSON(j, "Value", nlohmann::json()));
  min   = variantFromJSON(TPJSON(j, "Min",   nlohmann::json()));
  max   = variantFromJSON(TPJSON(j, "Max",   nlohmann::json()));
  step  = variantFromJSON(TPJSON(j, "Step",  nlohmann::json()));
}

//##################################################################################################
nlohmann::json Parameter::saveBinary(const std::function<uint64_t(const std::string&)>& addBlob) const
{
  nlohmann::json j;

  j["Name"]        = name.toString();
  j["Type"]        = type.toString();
  j["Min"]         = min             ;
  j["Max"]         = max             ;
  j["Step"]        = step            ;
  j["Mutate"]      = mutate          ;
  j["Description"] = description     ;

  if(type == binaryDataSID())
  {
    j["Value"]["type"] = "size_t";
    j["Value"]["value"] = size_t(addBlob(tpGetVariantValue<std::string>(value)));
  }
  else
    j["Value"] = value;

  return j;
}

//##################################################################################################
void Parameter::loadBinary(const nlohmann::json& j, const std::vector<std::string>& blobs)
{
  name        = TPJSONString(j, "Name"       );
  type        = TPJSONString(j, "Type"       );
  mutate      = TPJSONBool  (j, "Mutate"     );
  description = TPJSONString(j, "Description");

  value = variantFromJSON(TPJSON(j, "Value"));
  min   = variantFromJSON(TPJSON(j, "Min"  ));
  max   = variantFromJSON(TPJSON(j, "Max"  ));
  step  = variantFromJSON(TPJSON(j, "Step" ));

  if(type == binaryDataSID())
  {
    size_t index = tpGetVariantValue<size_t>(value);
    if(index<blobs.size())
      value = blobs.at(index);
  }
}

//##################################################################################################
nlohmann::json Parameter::saveHash(const std::unordered_map<tp_utils::StringID, tp_pipeline::Parameter>& paramHash)
{
  nlohmann::json j = nlohmann::json::array();

  for(const auto& v : paramHash)
    j.push_back(v.second.save());

  return j;
}

//##################################################################################################
std::unordered_map<tp_utils::StringID, tp_pipeline::Parameter> Parameter::loadHash(const nlohmann::json& j)
{
  std::unordered_map<tp_utils::StringID, tp_pipeline::Parameter> paramHash;
  for(Parameter param : j)
    if(param.name.isValid())
      paramHash[param.name] = param;

  return paramHash;
}

//##################################################################################################
void Parameter::setNamedData()
{
  type = tp_pipeline::namedDataSID();
  if(value.index() == 0)
    value = std::string("Output data");
}

}
