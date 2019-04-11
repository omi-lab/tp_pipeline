#ifndef tp_pipeline_PipelineManager_h
#define tp_pipeline_PipelineManager_h

#include "tp_pipeline/Globals.h"
#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include <unordered_map>
#include <memory>

namespace tp_data
{
class Collection;
class CollectionFactory;
}

namespace tp_pipeline
{

//##################################################################################################
//! This uses delegates to manage and execute a pipeline.
class PipelineManager
{
public:

  //################################################################################################
  PipelineManager(PipelineDetails* pipelineDetails,
                  const StepDelegateMap* stepDelegates,
                  const tp_data::CollectionFactory* collectionFactory,
                  bool fixupParameters=true);

  //################################################################################################
  ~PipelineManager();

  //################################################################################################
  //! This pushes data through the pipeline
  /*!
  This will clear out all cached data, combine this input with the static input and push that into
  the pipeline, and then return the results.

  \param errors - This will be populated with any errors encountered while processing.
  \param input - Data to push into the pipeline, this will be combined with the static input.
  \return The results produced by the pipeline.
  */
  std::shared_ptr<tp_data::Collection> execute(std::vector<std::string>& errors, const std::shared_ptr<tp_data::Collection>& input);

  //################################################################################################
  //! This pushes data through the pipeline
  /*!
  This is used to execute a pipeline where part of the data has been preprocessed it will 'skip' the
  steps that have been preprocessed and force the input data into the next step.

  \param errors - This will be populated with any errors encountered while processing.
  \param input - The preprocessed data to push into the next step.
  \param skip - The number of steps to skip.
  \return The results produced by the pipeline.
  */
  std::shared_ptr<tp_data::Collection> executeFrom(std::vector<std::string>& errors, const StepInput& input, size_t skip);

  //################################################################################################
  //! Execute the pipeline up to a given step and return the results
  std::shared_ptr<tp_data::Collection> executeStep(StepDetails* stepDetails);

  //################################################################################################
  //! Return the results passed into a given step
  StepInput stepInput(StepDetails* stepDetails);

  //################################################################################################
  //! Return the results passed into the step after a given step
  StepInput nextStepInput(StepDetails* stepDetails);

  //################################################################################################
  void generateNextStepInput(std::vector<std::string>& errors,
                             StepInput& result,
                             const std::shared_ptr<tp_data::Collection>& input,
                             StepDetails* stepDetails);

  //################################################################################################
  //! This sets the initial input that will get fed into the pipeline
  /*!
  This will clear all cached data and set the initial input for the pipeline.

  \param initialInput - This is combined with the input passed into each call to execute.
  */
  void setStaticInput(const std::shared_ptr<tp_data::Collection>& initialInput);

  //################################################################################################
  //! Returns the pipeline
  PipelineDetails* pipelineDetails();

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
