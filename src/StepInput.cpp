#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"

#include <numeric>

namespace tp_pipeline
{

//##################################################################################################
std::vector<tp_data::AbstractMember*> StepInput::members() const
{
  std::vector<tp_data::AbstractMember*> members;
  {
    size_t size{0};
    for(const auto& p : previousSteps)
      size+=p->members().size();
    members.reserve(size);
  }

  for(size_t i=previousSteps.size(); i>0; i--)
  {
    const auto& m = previousSteps.at(i-1)->members();
    members.insert(members.end(), m.begin(), m.end());
  }

  return members;
}

//##################################################################################################
tp_data::AbstractMember* StepInput::member(const tp_utils::StringID& name) const
{
  for(size_t i=previousSteps.size(); i>0; i--)
    if(auto m = previousSteps.at(i-1)->member(name); m)
      return m;
  return nullptr;
}

}
