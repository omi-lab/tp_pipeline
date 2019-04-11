#include "tp_pipeline/AbstractComplexObject.h"

namespace tp_pipeline
{

//##################################################################################################
AbstractComplexObject::AbstractComplexObject(const tp_utils::StringID& type):
  m_type(type)
{

}

//##################################################################################################
AbstractComplexObject::~AbstractComplexObject()
{

}

//##################################################################################################
tp_utils::StringID AbstractComplexObject::type()const
{
  return m_type;
}

}
