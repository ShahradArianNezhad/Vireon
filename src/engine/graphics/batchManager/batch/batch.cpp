#include "./batch.hpp"
#include <cstdint>



void Batch::remove(EntityId id){
  size_t index=UINT32_MAX;
  for(size_t i=0;i<entities.size();i++){
    if(entities[i]==id){
      index=i;
      break;
    }
  }
  if(index==UINT32_MAX){
    LOG_ERROR("trying to remove non exisiting entity in batch");
    return;
  }
  if(index==entities.size()){
    entities.pop_back();
    transformInstances.pop_back();
    colorInstances.pop_back();
    if(uvInstances.size()>0)uvInstances.pop_back();
  }else{
    entities[index]=entities[entities.size()-1];
    entities.pop_back();
    transformInstances[index]=transformInstances[transformInstances.size()-1];
    transformInstances.pop_back();
    colorInstances[index]=colorInstances[colorInstances.size()-1];
    colorInstances.pop_back();
    if(uvInstances.size()>0){
      uvInstances[index]=uvInstances[uvInstances.size()-1];
      uvInstances.pop_back();
    }
  }
}



void Batch::replaceModel(EntityId id,mat4 transform){
  size_t index=UINT32_MAX;
  for(size_t i=0;i<entities.size();i++){
    if(entities[i]==id){
      index=i;
      break;
    }
  }
  if(index==UINT32_MAX){
    return;
  }
  transformInstances[index]=transform;
}

void Batch::replaceColor(EntityId id,vec4 color){
  size_t index=UINT32_MAX;
  for(size_t i=0;i<entities.size();i++){
    if(entities[i]==id){
      index=i;
      break;
    }
  }
  if(index==UINT32_MAX){
    return;
  }
  colorInstances[index]=color;
};

void Batch::replaceUv(EntityId id,vec4 uv){
  size_t index=UINT32_MAX;
  for(size_t i=0;i<entities.size();i++){
    if(entities[i]==id){
      index=i;
      break;
    }
  }
  if(index==UINT32_MAX){
    return;
  }
  uvInstances[index]=uv;
};
