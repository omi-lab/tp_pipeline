#ifndef tp_pipeline_CollectionComplexObject_h
#define tp_pipeline_CollectionComplexObject_h

#include "tp_pipeline/AbstractComplexObject.h"
#include "tp_pipeline/AbstractComplexObjectFactory.h"

#include "tp_data/Collection.h"

#include "json.hpp"

namespace tp_pipeline
{

//##################################################################################################
class CollectionComplexObject: public AbstractComplexObject
{
public:
  //################################################################################################
  CollectionComplexObject(const tp_data::CollectionFactory* collectionFactory);

  //################################################################################################
  ~CollectionComplexObject() override;

  //################################################################################################
  nlohmann::json saveBinary(const std::function<uint64_t(const std::string&)>& addBlob) const override;

  //################################################################################################
  AbstractComplexObject* clone() const override;

  std::shared_ptr<tp_data::Collection> data;

private:
  const tp_data::CollectionFactory* m_collectionFactory;
};

//##################################################################################################
class CollectionComplexObjectFactory: public AbstractComplexObjectFactory
{
public:
  //################################################################################################
  CollectionComplexObjectFactory(const tp_data::CollectionFactory* collectionFactory);

  //################################################################################################
  AbstractComplexObject* create() const override;

  //################################################################################################
  AbstractComplexObject* loadBinary(const nlohmann::json& j, const std::vector<std::string>& blobs) const override;

private:
  const tp_data::CollectionFactory* m_collectionFactory;
};

}

#endif

