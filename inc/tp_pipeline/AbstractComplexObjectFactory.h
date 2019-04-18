#ifndef tp_pipeline_AbstractComplexObjectFactory_h
#define tp_pipeline_AbstractComplexObjectFactory_h

#include "tp_pipeline/Globals.h"

#include "json.hpp"

namespace tp_pipeline
{
class AbstractComplexObject;

//##################################################################################################
class AbstractComplexObjectFactory
{
  AbstractComplexObjectFactory( const AbstractComplexObjectFactory& ) = delete;
  AbstractComplexObjectFactory& operator=(const AbstractComplexObjectFactory&) = delete;
public:
  //################################################################################################
  AbstractComplexObjectFactory() = default;

  //################################################################################################
  virtual ~AbstractComplexObjectFactory();

  //################################################################################################
  virtual AbstractComplexObject* create()const=0;

  //################################################################################################
  virtual AbstractComplexObject* loadBinary(const nlohmann::json& j, const std::vector<std::string>& blobs)=0;
};

}

#endif
