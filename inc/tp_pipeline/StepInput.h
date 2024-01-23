#ifndef tp_pipeline_StepInput_h
#define tp_pipeline_StepInput_h

#include "tp_pipeline/Globals.h"

#include <memory>

namespace tp_data
{
class Collection;
class AbstractMember;
}

namespace tp_pipeline
{

//##################################################################################################
struct StepInput
{
  std::vector<std::shared_ptr<tp_data::Collection>> previousSteps;

  //################################################################################################
  std::vector<tp_data::AbstractMember*> members() const;

  //################################################################################################
  //! Find an member.
  /*!
  Search the results of the previous steps for a member.
  \note The Collection owns the returned member.
  \param name The unique name of the member to find.
  \returns A pointer to the member or nullptr.
  */
  tp_data::AbstractMember* member(const tp_utils::StringID& name) const;

  //################################################################################################
  template<typename T>
  void memberCast(const tp_utils::StringID& name, T*& obs) const
  {
    obs = dynamic_cast<T*>(member(name));
  }
};

}

#endif







//#include "tp_pipeline/StepDetails.h"

//#include "tp_image_utils/Point.h"
//#include "tp_image_utils/Grid.h"
//#include "tp_image_utils/ByteMap.h"




//  StepInput() = default;
//  StepInput(const StepInput& other) = delete;
//  StepInput& operator=(const StepInput& other) = delete;

//  ~StepInput();

//  std::shared_ptr<tp_data::Collection> noPreviousStep_;
//  std::vector<std::shared_ptr<tp_data::Collection>*> previousSteps;

//  std::unordered_map<std::string, const tp_image_utils::ColorMap*>             namedColorImages;
//  std::unordered_map<std::string, const tp_image_utils::ByteMap*>              namedByteMaps;
//  std::unordered_map<std::string, const tp_image_utils::LineCollection*>       namedLines;
//  std::unordered_map<std::string, const tp_image_utils::Grid*>                 namedGrids;
//  std::unordered_map<std::string, const std::string*>                          namedStrings;
//  std::unordered_map<std::string, const std::vector<std::string>*>             namedStringLists;
//  std::unordered_map<std::string, const std::vector<float>*>                   namedFloatVectors;
//  std::unordered_map<std::string, const std::vector<tp_math_utils::Polygon>*>  namedPolygons;

//  //################################################################################################
//  void deepCopy(const StepInput& other);

//  //################################################################################################
//  const std::shared_ptr<tp_data::Collection>& previousStep()const;

//  //################################################################################################
//  const tp_image_utils::ColorMap* findColorImage(const std::string& name)const;

//  //################################################################################################
//  const tp_image_utils::ByteMap* findByteMap(const std::string& name)const;

//  //################################################################################################
//  const tp_image_utils::LineCollection* findLines(const std::string& name)const;

//  //################################################################################################
//  const tp_image_utils::Grid* findGrid(const std::string& name)const;

//  //################################################################################################
//  const std::string* findString(const std::string& name)const;

//  //################################################################################################
//  const std::vector<std::string>* findStringList(const std::string& name)const;

//  //################################################################################################
//  const std::vector<float>* findFloatVector(const std::string& name)const;

//  //################################################################################################
//  const std::vector<tp_math_utils::Polygon>* findPolygons(const std::string& name)const;




