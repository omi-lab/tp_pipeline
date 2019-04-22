#include "tp_pipeline/ComplexObjectManager.h"
#include "tp_pipeline/AbstractComplexObject.h"
#include "tp_pipeline/AbstractComplexObjectFactory.h"

#include "tp_utils/JSONUtils.h"
#include "tp_utils/DebugUtils.h"

namespace tp_pipeline
{

//##################################################################################################
struct ComplexObjectManager::Private
{
  std::unordered_map<tp_utils::StringID, AbstractComplexObject*> complexObjects;
  std::unordered_map<tp_utils::StringID, nlohmann::json> complexObjectsJSON;
  std::unordered_map<tp_utils::StringID, std::vector<std::string>> complexObjectsBlobs;
};

//##################################################################################################
ComplexObjectManager::ComplexObjectManager():
  d(new Private)
{

}

//##################################################################################################
ComplexObjectManager::ComplexObjectManager(const ComplexObjectManager& other):
  d(new Private)
{
  d->complexObjectsJSON = other.d->complexObjectsJSON;
  d->complexObjectsBlobs = other.d->complexObjectsBlobs;

  for(const auto& i : other.d->complexObjects)
    d->complexObjects[i.first] = i.second->clone();
}

//##################################################################################################
ComplexObjectManager::~ComplexObjectManager()
{
  clearComplexObjects();
  delete d;
}

//##################################################################################################
nlohmann::json ComplexObjectManager::saveBinary(const std::function<uint64_t(const std::string&)>& addBlob) const noexcept
{
  nlohmann::json j;
  auto& objects   = j["objects"];
  auto& blobIndex = j["blobIndex"];

  objects   = nlohmann::json::array();
  blobIndex = nlohmann::json::array();

  std::string currentObjectName;
  std::unordered_map<std::string, uint64_t> countsPerObject;
  auto addIndexedBlob = [&](const std::string& data)
  {
    auto index = countsPerObject[currentObjectName]++;
    nlohmann::json jj;
    jj["index"] = addBlob(data);
    jj["name"]  = currentObjectName;
    blobIndex.push_back(jj);
    return index;
  };

  for(const auto& i : d->complexObjects)
  {
    currentObjectName = i.first.keyString();
    objects.push_back({{"name", i.first.keyString()}, {"data", i.second->saveBinary(addIndexedBlob)}});
  }

  for(const auto& i : d->complexObjectsJSON)
    objects.push_back({{"name", i.first.keyString()}, {"data", i.second}});

  for(const auto& i : d->complexObjectsBlobs)
  {
    currentObjectName = i.first.keyString();
    for(const auto& blob : i.second)
      addIndexedBlob(blob);
  }

  return j;
}

//##################################################################################################
void ComplexObjectManager::loadBinary(const nlohmann::json& j, const std::vector<std::string>& blobs) noexcept
{
  clearComplexObjects();

  if(auto blobIndex = TPJSON(j, "blobIndex"); blobIndex.is_array())
    for(const auto& jj : blobIndex)
      if(auto index = TPJSONSizeT(jj, "index"); index<blobs.size())
        d->complexObjectsBlobs[TPJSONString(jj, "name")].push_back(blobs.at(index));

  auto objects = TPJSON(j, "objects");
  if(objects.is_array())
  {
    for(nlohmann::json obj : objects)
    {
      tp_utils::StringID name = tp_utils::getJSONValue<std::string>(obj, "name", "");
      if(name.isValid())
        d->complexObjectsJSON[name] = obj["data"];
    }
  }
}

//##################################################################################################
void ComplexObjectManager::clearComplexObjects()
{
  for(const auto& i : d->complexObjects)
    delete i.second;

  d->complexObjectsJSON.clear();  
  d->complexObjectsBlobs.clear();
  d->complexObjects.clear();
}

//##################################################################################################
void ComplexObjectManager::removeComplexObject(const tp_utils::StringID& name)
{
  d->complexObjectsJSON.erase(name);
  d->complexObjectsBlobs.erase(name);

  auto i = d->complexObjects.find(name);
  if(i!=d->complexObjects.end())
  {
    delete i->second;
    d->complexObjects.erase(i);
  }
}

//##################################################################################################
AbstractComplexObject* ComplexObjectManager::complexObject(const tp_utils::StringID& name, AbstractComplexObjectFactory* factory)
{
  {
    auto i = d->complexObjects.find(name);
    if(i!=d->complexObjects.end())
      return i->second;
  }

  AbstractComplexObject* obj = nullptr;
  {
    auto i = d->complexObjectsJSON.find(name);
    if(i!=d->complexObjectsJSON.end())
    {
      obj = factory->loadBinary(i->second, d->complexObjectsBlobs[name]);
      d->complexObjectsJSON.erase(i);

      if(auto i = d->complexObjectsBlobs.find(name); i!=d->complexObjectsBlobs.end())
        d->complexObjectsBlobs.erase(i);
    }
  }

  if(!obj)
    obj = factory->create();

  if(obj)
    d->complexObjects[name] = obj;

  return obj;
}

}
