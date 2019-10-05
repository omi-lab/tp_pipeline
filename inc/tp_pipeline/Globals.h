#ifndef tp_pipeline_Globals_h
#define tp_pipeline_Globals_h

#include "tp_utils/StringID.h"

namespace tp_data
{
class CollectionFactory;
}

//##################################################################################################
//! A pipeline for data processing.
namespace tp_pipeline
{
class StepDelegateMap;
class StepDetails;

TP_DECLARE_ID(                         noneSID,                             "None");
TP_DECLARE_ID(                          intSID,                              "Int");
TP_DECLARE_ID(                         sizeSID,                             "Size");
TP_DECLARE_ID(                        floatSID,                            "Float");
TP_DECLARE_ID(                       doubleSID,                           "Double");
TP_DECLARE_ID(                         enumSID,                             "Enum");
TP_DECLARE_ID(                    namedDataSID,                       "Named data");
TP_DECLARE_ID(                    directorySID,                        "Directory");
TP_DECLARE_ID(                       stringSID,                           "String");
TP_DECLARE_ID(                   binaryDataSID,                      "Binary data");
TP_DECLARE_ID(            convolutionMatrixSID,               "Convolution matrix");
TP_DECLARE_ID(                fileDirectorySID,                   "File directory");
TP_DECLARE_ID(                    fileIndexSID,                       "File index");
TP_DECLARE_ID(                   collectionSID,                       "Collection");

//##################################################################################################
//! Add the step delegates that this module provides to the StepDelegateMap
void createStepDelegates(StepDelegateMap& stepDelegates, const tp_data::CollectionFactory* collectionFactory);

//##################################################################################################
//! Add all step delegates that have been registered to the StepDelegateMap
void createAllStepDelegates(StepDelegateMap& stepDelegates, const tp_data::CollectionFactory* collectionFactory);

//##################################################################################################
std::vector<std::function<void(StepDelegateMap&, const tp_data::CollectionFactory*)>>& createStepDelegatesRegister();

//##################################################################################################
//! Add this namspaces createCollectionFactories method to the global register.
#define REGISTER_CREATE_STEP_DELEGATES \
extern char createStepDelegates_reg; \
char createStepDelegates_reg = [] \
{ \
  tp_pipeline::createStepDelegatesRegister().emplace_back(createStepDelegates); \
  return 0; \
}()

//##################################################################################################
//! Sets the input directory for a sequence of steps
/*!
This will look for the first that has a "File directory" parameter and set it.

\param steps - The list of steps to modify
\param directory - The input directory
*/
void setInputDirectory(const std::vector<StepDetails*>& steps, const std::string& directory);

//##################################################################################################
//! Static initialization of this module, see TP_STATIC_INIT in dependencies.pri
int staticInit();

}

#endif
