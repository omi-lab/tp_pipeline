#include "tp_pipeline/complex_objects/CollectionComplexObject.h"

#include "tp_data/CollectionFactory.h"

#include "tp_utils/JSONUtils.h"
#include "tp_utils/DebugUtils.h"

namespace tp_pipeline
{

//##################################################################################################
CollectionComplexObject::CollectionComplexObject(const tp_data::CollectionFactory* collectionFactory):
  AbstractComplexObject(collectionSID()),
  m_collectionFactory(collectionFactory)
{

}

//##################################################################################################
CollectionComplexObject::~CollectionComplexObject() = default;

//##################################################################################################
nlohmann::json CollectionComplexObject::saveBinary(const std::function<uint64_t(const std::string&)>& addBlob) const
{
  nlohmann::json j;

  std::string error;
  std::string binaryData;
  if(data)
  {
    m_collectionFactory->saveToData(error, *data, binaryData);
    if(!error.empty())
      tpWarning() << "CollectionComplexObject::saveBinary Error: " << error;
  }

  j["index"] = addBlob(binaryData);

  return j;
}

//##################################################################################################
AbstractComplexObject* CollectionComplexObject::clone() const
{
  auto obj = new CollectionComplexObject(m_collectionFactory);
  obj->data = data;
  return obj;
}
//##################################################################################################
CollectionComplexObjectFactory::CollectionComplexObjectFactory(const tp_data::CollectionFactory* collectionFactory):
  m_collectionFactory(collectionFactory)
{

}

//##################################################################################################
AbstractComplexObject* CollectionComplexObjectFactory::create() const
{
  return new CollectionComplexObject(m_collectionFactory);
}

//##################################################################################################
AbstractComplexObject* CollectionComplexObjectFactory::loadBinary(const nlohmann::json& j, const std::vector<std::string>& blobs)
{
  auto obj = new CollectionComplexObject(m_collectionFactory);
  obj->data = std::make_shared<tp_data::Collection>();

  size_t index = TPJSONSizeT(j, "index");
  if(index<blobs.size())
  {
    const std::string& data = blobs.at(index);

    std::string error;
    m_collectionFactory->loadFromData(error, data, *obj->data);
    if(!error.empty())
      tpWarning() << "CollectionComplexObjectFactory failed to load: " << error;
  }

  return obj;
}

}
