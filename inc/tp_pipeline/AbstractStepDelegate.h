#ifndef tp_pipeline_AbstractStepDelegate_h
#define tp_pipeline_AbstractStepDelegate_h

#include "tp_pipeline/Globals.h"

#include <memory>

namespace tp_data
{
class Collection;
}

namespace tp_pipeline
{
struct StepInput;
class StepDetails;

//##################################################################################################
class AbstractStepDelegate
{
public:
  //################################################################################################
  AbstractStepDelegate(const tp_utils::StringID& name, const std::vector<tp_utils::StringID>& groups);

  //################################################################################################
  virtual ~AbstractStepDelegate();

  //################################################################################################
  const tp_utils::StringID& name()const;

  //################################################################################################
  const std::vector<tp_utils::StringID>& groups() const;

  //################################################################################################
  virtual void executeStep(StepDetails* stepDetails, const StepInput& input, tp_data::Collection& output) const=0;

  //################################################################################################
  //! This gets called to add, adapt, and validate step parametes
  /*!
  This should add any parameters to the step that are missing, remove any that are not needed, and
  alter any that are not valid. This will be called on the blan step to add the default set of
  parameters, it will then be called each time the step is altered to make sure that it has a valid
  set of parameters.
  \param stepDetails
  */
  virtual void fixupParameters(StepDetails* stepDetails) const=0;

private:
  struct Private;
  friend struct Private;
  Private* d;
};

}

#endif
