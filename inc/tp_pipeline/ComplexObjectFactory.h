#pragma once

#include "tp_pipeline/Globals.h"

#include "json.hpp" // IWYU pragma: keep

namespace tp_pipeline
{
class ComplexObject;

//##################################################################################################
class TP_PIPELINE_SHARED_EXPORT ComplexObjectFactory
{
  ComplexObjectFactory( const ComplexObjectFactory& ) = delete;
  ComplexObjectFactory& operator=(const ComplexObjectFactory&) = delete;
public:
  //################################################################################################
  ComplexObjectFactory() = default;

  //################################################################################################
  virtual ~ComplexObjectFactory();

  //################################################################################################
  virtual ComplexObject* create() const=0;

  //################################################################################################
  virtual ComplexObject* loadBinary(const nlohmann::json& j, const std::vector<std::string>& blobs) const=0;
};

}
