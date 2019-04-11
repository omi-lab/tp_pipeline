#ifndef tp_pipeline_ComplexObjectManager_h
#define tp_pipeline_ComplexObjectManager_h

#include "tp_pipeline/Globals.h"
#include "tp_pipeline/Parameter.h"

#include "tp_utils/RefCount.h"

#include "json.hpp"

#include <unordered_map>

namespace tp_pipeline
{

class AbstractComplexObjectFactory;
class AbstractComplexObject;

//##################################################################################################
class ComplexObjectManager
{
public:
  //################################################################################################
  ComplexObjectManager();

  //################################################################################################
  ComplexObjectManager(const ComplexObjectManager& other);

  //################################################################################################
  ~ComplexObjectManager();

  //################################################################################################
  //! Saves the state of a step
  nlohmann::json save()noexcept;

  //################################################################################################
  //! loads the state of a step
  void load(const nlohmann::json& j)noexcept;

  //################################################################################################
  void clearComplexObjects();

  //################################################################################################
  void removeComplexObject(const tp_utils::StringID& name);

  //################################################################################################
  //! Add or create a complex object
  /*!
  This method will do 1 of three things:
  - If it has a complex object of this type it will return it.
  - If it has json for a complex object it will call the load method on the factory.
  - Else it will call the create method on the factory.
  In all three cases the complex object will be added to the internal map and returned.

  You should dynamic cast the result and check for nullptr, if you dont get what you expect you
  should probably call clearComplexObjects() and create a fresh set of complex objects. This is
  likely to be caused when the step is changed from one type to another.

  \param name - The name to store the complex object as.
  \param factory - The factory to use to load or create the complex object.
  \return The complex object. Dynamic cast the result and check for nullptr.
  */
  AbstractComplexObject* complexObject(const tp_utils::StringID& name, AbstractComplexObjectFactory* factory);

private:
  struct Private;
  friend struct Private;
  Private* d;
};


}

#endif
