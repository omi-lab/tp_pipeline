#include "tp_pipeline/ComplexObjectManager.h"
#include "tp_pipeline/AbstractComplexObject.h"
#include "tp_pipeline/AbstractComplexObjectFactory.h"

#include "tp_utils/JSONUtils.h"

namespace tp_pipeline
{

//##################################################################################################
struct ComplexObjectManager::Private
{
  std::unordered_map<tp_utils::StringID, AbstractComplexObject*> complexObjects;
  std::unordered_map<tp_utils::StringID, nlohmann::json> complexObjectsJSON;
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
nlohmann::json ComplexObjectManager::save() noexcept
{
  nlohmann::json j = nlohmann::json::array();

  for(const auto& i : d->complexObjects)
    j.push_back({{"name", i.first.keyString()}, {"data", i.second->save()}});
  for(const auto& i : d->complexObjectsJSON)
    j.push_back({{"name", i.first.keyString()}, {"data", i.second}});

  return j;
}

//##################################################################################################
void ComplexObjectManager::load(const nlohmann::json& j) noexcept
{
  clearComplexObjects();
  for(nlohmann::json obj : j)
  {
    tp_utils::StringID name = tp_utils::getJSONValue<std::string>(obj, "name", "");
    if(name.isValid())
      d->complexObjectsJSON[name] = obj["data"];
  }
}

//##################################################################################################
void ComplexObjectManager::clearComplexObjects()
{
  for(const auto& i : d->complexObjects)
    delete i.second;

  d->complexObjectsJSON.clear();
  d->complexObjects.clear();
}

//##################################################################################################
void ComplexObjectManager::removeComplexObject(const tp_utils::StringID& name)
{
  d->complexObjectsJSON.erase(name);

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
      obj = factory->load(i->second);
      d->complexObjectsJSON.erase(i);
    }
  }

  if(!obj)
    obj = factory->create();

  if(obj)
    d->complexObjects[name] = obj;

  return obj;
}

}
