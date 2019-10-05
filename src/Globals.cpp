#include "tp_pipeline/Globals.h"
#include "tp_pipeline/StepDelegateMap.h"
#include "tp_pipeline/Parameter.h"
#include "tp_pipeline/StepDetails.h"

#include "tp_pipeline/step_delegates/NoneStepDelegate.h"

#include "tp_data/CollectionFactory.h"

#include "tp_utils/DebugUtils.h"

//##################################################################################################
namespace tp_pipeline
{
tp_qt_DEFINE_ID(                         noneSID,                             "None");
tp_qt_DEFINE_ID(                          intSID,                              "Int");
tp_qt_DEFINE_ID(                         sizeSID,                             "Size");
tp_qt_DEFINE_ID(                        floatSID,                            "Float");
tp_qt_DEFINE_ID(                       doubleSID,                           "Double");
tp_qt_DEFINE_ID(                         enumSID,                             "Enum");
tp_qt_DEFINE_ID(                    namedDataSID,                       "Named data");
tp_qt_DEFINE_ID(                    directorySID,                        "Directory");
tp_qt_DEFINE_ID(                       stringSID,                           "String");
tp_qt_DEFINE_ID(                   binaryDataSID,                      "Binary data");
tp_qt_DEFINE_ID(            convolutionMatrixSID,               "Convolution matrix");
tp_qt_DEFINE_ID(                fileDirectorySID,                   "File directory");
tp_qt_DEFINE_ID(                    fileIndexSID,                       "File index");
tp_qt_DEFINE_ID(                   collectionSID,                       "Collection");

//##################################################################################################
void createStepDelegates(StepDelegateMap& stepDelegates, const tp_data::CollectionFactory* collectionFactory)
{
  TP_UNUSED(collectionFactory);
  stepDelegates.addStepDelegate(new NoneStepDelegate);
}

//##################################################################################################
void createAllStepDelegates(StepDelegateMap& stepDelegates, const tp_data::CollectionFactory* collectionFactory)
{
  if(!collectionFactory->finalized())
    tpWarning() << "createAllStepDelegates Error: The collection factory should be finalized!";

  for(const auto& createStepDelegates : tp_pipeline::createStepDelegatesRegister())
    createStepDelegates(stepDelegates, collectionFactory);
}

//##################################################################################################
std::vector<std::function<void(StepDelegateMap&, const tp_data::CollectionFactory*) > > & createStepDelegatesRegister()
{
  static std::vector<std::function<void(StepDelegateMap&, const tp_data::CollectionFactory*)>> createStepDelegatesRegister;
  return createStepDelegatesRegister;
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

REGISTER_CREATE_STEP_DELEGATES;

//##################################################################################################
int staticInit()
{
  return 0;
}

}
