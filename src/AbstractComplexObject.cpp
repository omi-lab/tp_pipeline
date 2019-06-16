#include "tp_pipeline/AbstractComplexObject.h"

namespace tp_pipeline
{

//##################################################################################################
AbstractComplexObject::AbstractComplexObject(tp_utils::StringID type):
  m_type(std::move(type))
{

}

//##################################################################################################
AbstractComplexObject::~AbstractComplexObject() = default;

//##################################################################################################
tp_utils::StringID AbstractComplexObject::type()const
{
  return m_type;
}

}
