#pragma once

#include "tp_pipeline/ComplexObject.h"
#include "tp_pipeline/ComplexObjectFactory.h"

#include "tp_data/Collection.h"

#include "json.hpp"

namespace tp_pipeline
{

//##################################################################################################
class CollectionComplexObject: public ComplexObject
{
public:
  //################################################################################################
  CollectionComplexObject(const tp_data::CollectionFactory* collectionFactory);

  //################################################################################################
  ~CollectionComplexObject() override;

  //################################################################################################
  void saveBinary(nlohmann::json& j,const std::function<uint64_t(const std::string&)>& addBlob) const override;

  //################################################################################################
  ComplexObject* clone() const override;

  std::shared_ptr<tp_data::Collection> data;

private:
  const tp_data::CollectionFactory* m_collectionFactory;
};

//##################################################################################################
class CollectionComplexObjectFactory: public ComplexObjectFactory
{
public:
  //################################################################################################
  CollectionComplexObjectFactory(const tp_data::CollectionFactory* collectionFactory);

  //################################################################################################
  ComplexObject* create() const override;

  //################################################################################################
  ComplexObject* loadBinary(const nlohmann::json& j, const std::vector<std::string>& blobs) const override;

private:
  const tp_data::CollectionFactory* m_collectionFactory;
};

}
