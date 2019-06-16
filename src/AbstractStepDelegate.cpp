#include "tp_pipeline/AbstractStepDelegate.h"

namespace tp_pipeline
{
//##################################################################################################
struct AbstractStepDelegate::Private
{
  const tp_utils::StringID name;
  const std::vector<tp_utils::StringID> groups;

  //################################################################################################
  Private(tp_utils::StringID name_,
          std::vector<tp_utils::StringID> groups_):
    name(std::move(name_)),
    groups(std::move(groups_))
  {

  }
};

//##################################################################################################
AbstractStepDelegate::AbstractStepDelegate(const tp_utils::StringID& name,
                                           const std::vector<tp_utils::StringID>& groups):
  d(new Private(name, groups))
{

}

//##################################################################################################
AbstractStepDelegate::~AbstractStepDelegate()
{
  delete d;
}

//##################################################################################################
const tp_utils::StringID& AbstractStepDelegate::name()const
{
  return d->name;
}

//################################################################################################
const std::vector<tp_utils::StringID>& AbstractStepDelegate::groups()const
{
  return d->groups;
}

}
