#include "tp_pipeline/Globals.h"
#include "tp_pipeline/StepDelegateMap.h"
#include "tp_pipeline/Parameter.h"
#include "tp_pipeline/StepDetails.h"

#include "tp_pipeline/step_delegates/NoneStepDelegate.h"

//##################################################################################################
namespace tp_pipeline
{
TDP_DEFINE_ID(                         noneSID,                             "None")
TDP_DEFINE_ID(                          intSID,                              "Int")
TDP_DEFINE_ID(                         sizeSID,                             "Size")
TDP_DEFINE_ID(                        floatSID,                            "Float")
TDP_DEFINE_ID(                       doubleSID,                           "Double")
TDP_DEFINE_ID(                         enumSID,                             "Enum")
TDP_DEFINE_ID(                    namedDataSID,                       "Named data")
TDP_DEFINE_ID(                    directorySID,                        "Directory")
TDP_DEFINE_ID(                       stringSID,                           "String")
TDP_DEFINE_ID(                   binaryDataSID,                      "Binary data")
TDP_DEFINE_ID(            convolutionMatrixSID,               "Convolution matrix")
TDP_DEFINE_ID(                fileDirectorySID,                   "File directory")
TDP_DEFINE_ID(                    fileIndexSID,                       "File index")
TDP_DEFINE_ID(                   collectionSID,                       "Collection")

//##################################################################################################
void createStepDelegates(StepDelegateMap& stepDelegates)
{
  stepDelegates.addStepDelegate(new NoneStepDelegate);
}

//##################################################################################################
void setInputDirectory(const std::vector<StepDetails*>& steps, const std::string& directory)
{
  const auto& name = fileDirectorySID();
  for(tp_pipeline::StepDetails* stepDetails : steps)
  {
    const std::unordered_map<tp_utils::StringID, Parameter>& parameters = stepDetails->parameters();
    if(tpContainsKey(parameters, name))
    {
      tp_pipeline::Parameter param = tpGetMapValue(parameters, name);
      param.name = name;
      param.value = directory;
      stepDetails->setParamerter(param);
      break;
    }
  }
}

}
