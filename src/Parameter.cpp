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

  j["Name"]        = name.keyString();
  j["Type"]        = type.keyString();
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
  name        = tp_utils::getJSONValue                (j, "Name",        std::string()   );
  type        = tp_utils::getJSONValue                (j, "Type",        std::string()   );
  mutate      = tp_utils::getJSONValue                (j, "Mutate",      false           );
  description = tp_utils::getJSONValue                (j, "Description", std::string()   );

  value = variantFromJSON(tp_utils::getJSONValue<nlohmann::json>(j, "Value", nlohmann::json()));
  min   = variantFromJSON(tp_utils::getJSONValue<nlohmann::json>(j, "Min",   nlohmann::json()));
  max   = variantFromJSON(tp_utils::getJSONValue<nlohmann::json>(j, "Max",   nlohmann::json()));
  step  = variantFromJSON(tp_utils::getJSONValue<nlohmann::json>(j, "Step",  nlohmann::json()));
}

//##################################################################################################
nlohmann::json Parameter::saveBinary(const std::function<uint64_t(const std::string&)>& addBlob) const
{
  nlohmann::json j;

  j["Name"]        = name.keyString();
  j["Type"]        = type.keyString();
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
  name        = tp_utils::getJSONValue                (j, "Name",        std::string()   );
  type        = tp_utils::getJSONValue                (j, "Type",        std::string()   );
  mutate      = tp_utils::getJSONValue                (j, "Mutate",      false           );
  description = tp_utils::getJSONValue                (j, "Description", std::string()   );

  value = variantFromJSON(tp_utils::getJSONValue<nlohmann::json>(j, "Value", nlohmann::json()));
  min   = variantFromJSON(tp_utils::getJSONValue<nlohmann::json>(j, "Min",   nlohmann::json()));
  max   = variantFromJSON(tp_utils::getJSONValue<nlohmann::json>(j, "Max",   nlohmann::json()));
  step  = variantFromJSON(tp_utils::getJSONValue<nlohmann::json>(j, "Step",  nlohmann::json()));

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

}
