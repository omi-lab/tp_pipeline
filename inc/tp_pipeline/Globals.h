#ifndef tp_pipeline_Globals_h
#define tp_pipeline_Globals_h

#include "tp_utils/StringID.h"

//##################################################################################################
//! A pipeline for data processing.
namespace tp_pipeline
{
class StepDelegateMap;
class StepDetails;

TDP_DECLARE_ID(                         noneSID,                             "None")
TDP_DECLARE_ID(                          intSID,                              "Int")
TDP_DECLARE_ID(                         sizeSID,                             "Size")
TDP_DECLARE_ID(                        floatSID,                            "Float")
TDP_DECLARE_ID(                       doubleSID,                           "Double")
TDP_DECLARE_ID(                         enumSID,                             "Enum")
TDP_DECLARE_ID(                    namedDataSID,                       "Named data")
TDP_DECLARE_ID(                    directorySID,                        "Directory")
TDP_DECLARE_ID(                       stringSID,                           "String")
TDP_DECLARE_ID(                   binaryDataSID,                      "Binary data")
TDP_DECLARE_ID(            convolutionMatrixSID,               "Convolution matrix")
TDP_DECLARE_ID(                fileDirectorySID,                   "File directory")
TDP_DECLARE_ID(                    fileIndexSID,                       "File index")

//##################################################################################################
//! Add the step delegates that this module provides to the StepDelegateMap
void createStepDelegates(StepDelegateMap& stepDelegates);

//##################################################################################################
//! Sets the input directory for a sequence of steps
/*!
This will look for the first that has a "File directory" parameter and set it.

\param steps - The list of steps to modify
\param directory - The input directory
*/
void setInputDirectory(const std::vector<StepDetails*>& steps, const std::string& directory);

}

#endif
