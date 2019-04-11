#ifndef tp_pipeline_AbstractComplexObjectFactory_h
#define tp_pipeline_AbstractComplexObjectFactory_h

#include "tp_pipeline/Globals.h"

#include "json.hpp"

namespace tp_pipeline
{

//##################################################################################################
class AbstractComplexObjectFactory
{
  AbstractComplexObjectFactory( const AbstractComplexObjectFactory& ) = delete;
  AbstractComplexObjectFactory& operator=(const AbstractComplexObjectFactory&) = delete;
public:
  //################################################################################################
  AbstractComplexObjectFactory() = default;

  //################################################################################################
  virtual ~AbstractComplexObjectFactory() = default;

  //################################################################################################
  virtual AbstractComplexObject* create()const=0;

  //################################################################################################
  virtual AbstractComplexObject* load(const nlohmann::json& j)const=0;
};

}

#endif
