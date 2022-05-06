#include "tp_pipeline/PipelineDetails.h"

#include "tp_utils/StackTrace.h"
#include "tp_utils/DebugUtils.h"
#include "tp_utils/JSONUtils.h"

namespace tp_pipeline
{

//##################################################################################################
struct PipelineDetails::Private
{
  TP_REF_COUNT_OBJECTS("PipelineDetails");
  PipelineDetails* q;

  std::vector<StepDetails*> steps;

  //################################################################################################
  Private(PipelineDetails* q_):
    q(q_)
  {

  }

  //################################################################################################
  ~Private()
  {
    if(!steps.empty())
      tp_utils::printStackTrace();
  }

  //################################################################################################
  void copy(const PipelineDetails& other)
  {
    for(StepDetails* step : other.d->steps)
    {
      StepDetails* newStep = new StepDetails(*step);
      newStep->setParent(q);
      steps.push_back(newStep);
    }
  }
};

//##################################################################################################
PipelineDetails::PipelineDetails():
  d(new Private(this))
{

}

//##################################################################################################
PipelineDetails::PipelineDetails(const PipelineDetails& other):
  d(new Private(this))
{  
  d->copy(other);
}

//##################################################################################################
PipelineDetails::~PipelineDetails()
{
  for(StepDetails* step : d->steps)
    delete step;
  d->steps.clear();
  delete d;
}

//################################################################################################
PipelineDetails& PipelineDetails::operator=(const PipelineDetails& other)
{
  while(!d->steps.empty())
    deleteStep(d->steps.at(0));

  d->copy(other);

  for(size_t i=0; i<d->steps.size(); i++)
    invalidateStep(d->steps.at(i));

  return *this;
}

//##################################################################################################
std::vector<StepDetails*> PipelineDetails::steps()const
{
  return d->steps;
}

//##################################################################################################
void PipelineDetails::addStep(StepDetails* step)
{
  insertStep(step, d->steps.size());
}

//##################################################################################################
void PipelineDetails::insertStep(StepDetails* step, size_t index)
{
  size_t invalidateFrom=d->steps.size();

  if(step->parent())
  {
    if(step->parent() == this)
    {
      auto i = std::find(d->steps.begin(), d->steps.end(), step);
      if(i!=d->steps.end())
      {
        auto f = size_t(i-d->steps.begin());
        if(f<invalidateFrom)
          invalidateFrom=f;

        d->steps.erase(i);
      }
    }
    else
    {
      tpWarning() << "PipelineDetails::addStep() Error! Step already has a parent.";
      return;
    }
  }
  else
    step->setParent(this);

  if(index<invalidateFrom)
    invalidateFrom=index;

  d->steps.insert(d->steps.begin()+long(index), step);

  for(size_t i=invalidateFrom; i<d->steps.size(); i++)
    invalidateStep(d->steps.at(i));
}

//##################################################################################################
void PipelineDetails::invalidateStep(StepDetails* step)
{
  changedCallback(step);
}

//##################################################################################################
void PipelineDetails::deleteStep(StepDetails* step)
{
  auto it = std::find(d->steps.begin(), d->steps.end(), step);
  if(it!=d->steps.end())
    d->steps.erase(it);

  deletedCallback(step);

  delete step;
}

//##################################################################################################
void PipelineDetails::saveBinary(std::string& data)
{
  std::vector<std::string> blobs;
  auto addBlob = [&](const std::string& blob)
  {
    auto i=blobs.size();
    blobs.push_back(blob);
    return i;
  };

  nlohmann::json j;
  j["Steps"] = nlohmann::json::array();
  for(StepDetails* step : d->steps)
    j["Steps"].push_back(step->saveBinary(addBlob));

  addBlob(j.dump());

  uint64_t indexSize = (blobs.size()*sizeof(uint64_t)*2)+sizeof(uint64_t);
  std::string index;
  index.reserve(indexSize);
  auto addNumber = [&](uint64_t number)
  {
    index.push_back(static_cast<char>(uint8_t(number>> 0)));
    index.push_back(static_cast<char>(uint8_t(number>> 8)));
    index.push_back(static_cast<char>(uint8_t(number>>16)));
    index.push_back(static_cast<char>(uint8_t(number>>24)));

    index.push_back(static_cast<char>(uint8_t(number>>32)));
    index.push_back(static_cast<char>(uint8_t(number>>40)));
    index.push_back(static_cast<char>(uint8_t(number>>48)));
    index.push_back(static_cast<char>(uint8_t(number>>56)));
  };

  addNumber(blobs.size());

  uint64_t total = indexSize;
  for(const auto& blob : blobs)
  {
    addNumber(total);
    addNumber(blob.size());
    total += blob.size();
  }

  data.reserve(total);
  data.append(index);
  for(const auto& blob : blobs)
    data.append(blob);
}

//##################################################################################################
void PipelineDetails::loadBinary(std::string& error, const std::string& data)
{
  if(data.size()<sizeof(uint64_t))
  {
    error = "Data is smaller than the indexes headder.";
    return;
  }

  const char* indexPtr = data.data();
  auto readNumber = [&]()
  {
    uint64_t number=0;

    number |= uint64_t(uint8_t(*indexPtr))<< 0; indexPtr++;
    number |= uint64_t(uint8_t(*indexPtr))<< 8; indexPtr++;
    number |= uint64_t(uint8_t(*indexPtr))<<16; indexPtr++;
    number |= uint64_t(uint8_t(*indexPtr))<<24; indexPtr++;

    number |= uint64_t(uint8_t(*indexPtr))<<32; indexPtr++;
    number |= uint64_t(uint8_t(*indexPtr))<<40; indexPtr++;
    number |= uint64_t(uint8_t(*indexPtr))<<48; indexPtr++;
    number |= uint64_t(uint8_t(*indexPtr))<<56; indexPtr++;

    return number;
  };

  uint64_t numberOfBlobs = readNumber();
  if(numberOfBlobs<1)
  {
    error = "No blobs found in data.";
    return;
  }

  size_t indexSize = (numberOfBlobs*sizeof(uint64_t)*2)+sizeof(uint64_t);
  if(indexSize>data.size())
  {
    error = "Index size is larger than source data.";
    return;
  }

  std::vector<std::string> blobs;
  uint64_t total = indexSize;
  for(uint64_t b=0; b<numberOfBlobs; b++)
  {
    uint64_t blobOffset = readNumber();
    if(total != blobOffset)
    {
      error = "Blob offset missmatch.";
      return;
    }

    uint64_t blobSize = readNumber();
    total += blobSize;
    if(total > data.size())
    {
      error = "Blob overflow.";
      return;
    }

    blobs.push_back(data.substr(blobOffset, blobSize));
  }

  if(blobs.empty())
  {
    error = "Failed to read any blobs.";
    return;
  }

  {
    auto j = nlohmann::json::parse(blobs.back());

    while(!d->steps.empty())
      deleteStep(d->steps.at(0));

    if(auto i=j.find("Steps"); i!=j.end() && i->is_array())
    {
      for(const auto& stepData : *i)
      {
        StepDetails* step = new StepDetails();
        step->loadBinary(stepData, blobs);
        addStep(step);
      }
    }
  }
}

}
