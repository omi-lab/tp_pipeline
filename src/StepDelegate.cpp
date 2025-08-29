#include "tp_pipeline/StepDelegate.h"

namespace tp_pipeline
{
//##################################################################################################
struct StepDelegate::Private
{
  const tp_utils::StringID name;
  const std::vector<tp_utils::StringID> groups;

  const std::vector<PortDetails> inPorts;
  const std::vector<PortDetails> outPorts;

  //################################################################################################
  Private(const tp_utils::StringID& name_,
          const std::vector<tp_utils::StringID>& groups_,
          const std::vector<PortDetails>& inPorts_,
          const std::vector<PortDetails>& outPorts_):
    name(name_),
    groups(groups_),
    inPorts(inPorts_),
    outPorts(outPorts_)
  {

  }
};

//##################################################################################################
StepDelegate::StepDelegate(const tp_utils::StringID& name,
                                           const std::vector<tp_utils::StringID>& groups,
                                           const std::vector<PortDetails>& inPorts,
                                           const std::vector<PortDetails>& outPorts):
  d(new Private(name, groups, inPorts, outPorts))
{

}

//##################################################################################################
StepDelegate::~StepDelegate()
{
  delete d;
}

//##################################################################################################
const tp_utils::StringID& StepDelegate::name()const
{
  return d->name;
}

//##################################################################################################
const std::vector<tp_utils::StringID>& StepDelegate::groups()const
{
  return d->groups;
}

//##################################################################################################
const std::vector<PortDetails>& StepDelegate::inPorts() const
{
  return d->inPorts;
}

//##################################################################################################
const std::vector<PortDetails>& StepDelegate::outPorts() const
{
  return d->outPorts;
}

}
