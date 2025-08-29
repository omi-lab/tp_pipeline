#include "tp_pipeline/Globals.h"
#include "tp_pipeline/StepDelegateMap.h"
#include "tp_pipeline/Parameter.h"
#include "tp_pipeline/StepDetails.h"

#include "tp_pipeline/step_delegates/NoneStepDelegate.h"
#include "tp_pipeline/step_delegates/CopyStepDelegate.h"

#include "tp_data/CollectionFactory.h"

#include "tp_utils/DebugUtils.h"

//##################################################################################################
namespace tp_pipeline
{
TP_DEFINE_ID(                         noneSID,                             "None");
TP_DEFINE_ID(                         copySID,                             "Copy");
TP_DEFINE_ID(                          intSID,                              "Int");
TP_DEFINE_ID(                         sizeSID,                             "Size");
TP_DEFINE_ID(                        floatSID,                            "Float");
TP_DEFINE_ID(                       doubleSID,                           "Double");
TP_DEFINE_ID(                         enumSID,                             "Enum");
TP_DEFINE_ID(                    namedDataSID,                       "Named data");
TP_DEFINE_ID(                    directorySID,                        "Directory");
TP_DEFINE_ID(                       stringSID,                           "String");
TP_DEFINE_ID(                   binaryDataSID,                      "Binary data");
TP_DEFINE_ID(            convolutionMatrixSID,               "Convolution matrix");
TP_DEFINE_ID(                fileDirectorySID,                   "File directory");
TP_DEFINE_ID(                    fileIndexSID,                       "File index");
TP_DEFINE_ID(                   collectionSID,                       "Collection");
TP_DEFINE_ID(                  memberNamesSID,                     "Member names");

//##################################################################################################
void createStepDelegates(StepDelegateMap& stepDelegates, const tp_data::CollectionFactory* collectionFactory)
{
  TP_UNUSED(collectionFactory);
  stepDelegates.addStepDelegate(new NoneStepDelegate);
  stepDelegates.addStepDelegate(new CopyStepDelegate(collectionFactory));
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



//##################################################################################################
tp_utils::StringID randomId()
{
  static std::random_device randomDevice;
  static std::mt19937 generator{randomDevice()};
  static const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  std::uniform_int_distribution<> distribution(0, characters.size() - 1);

  std::string randomString;
  for (size_t i=0; i<15; i++)
    randomString += characters[distribution(generator)];

  return randomString;
}

REGISTER_CREATE_STEP_DELEGATES;

//##################################################################################################
int staticInit()
{
  return 0;
}

}
