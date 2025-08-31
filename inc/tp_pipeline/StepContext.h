#pragma once

#include "tp_pipeline/Globals.h"
#include "tp_pipeline/StepOutput.h"

#include "tp_data/Collection.h"

#include <unordered_set>

namespace tp_data
{
class Collection;
class CollectionFactory;
}

namespace tp_pipeline
{
class PipelineManager;
class StepDelegate;

//##################################################################################################
struct StepContext
{
  const StepDelegate* stepDelegate{nullptr};
  StepDetails* stepDetails{nullptr};

  std::unordered_set<StepContext*> dependsOn;

  std::shared_ptr<tp_data::Collection> stepInput;
  std::shared_ptr<StepOutput> stepOutput;

  bool runStarted{false};
  bool runComplete{false};

  //################################################################################################
  bool dependenciesMet() const;
};

}
