#pragma once
#include "engine/entityManager/component/components.hpp"
#include <cstddef>


template <typename T>
class ComponentAllocator{

private:
  std::vector<T> components;
  std::unordered_map<EntityId, size_t> entityToIndex;
  std::vector<size_t> indexToEntity;

public:
  T getComponent(EntityId id){
    if(entityToIndex.contains(id))return components[entityToIndex[id]];
    LOG_WARN("getComponent:{} called on entity:{} which doesnt have that component",typeid(T).name(),id);
    return T{};
  }


  void setComponent(EntityId id,T comp){
    if(entityToIndex.contains(id)){
      components[entityToIndex[id]] = comp;
      return;
    }
      components.push_back(comp);
      entityToIndex[id]=components.size()-1;
      indexToEntity.push_back(id);
  }

  bool hasComponent(EntityId id){return entityToIndex.contains(id);};

  void deleteComponent(EntityId id){
    if(!entityToIndex.contains(id)){
      LOG_WARN("trying to delete non existing component:{} on entity:{}",typeid(T).name(),id);
      return;
    }

    size_t index = entityToIndex[id];
    if(index==components.size()-1){
      components.pop_back();
      entityToIndex.erase(id);
      indexToEntity.pop_back();
    }else{
      components[index] = components[components.size()-1];
      components.pop_back();
      indexToEntity[index]=indexToEntity[components.size()-1];
      auto backId = indexToEntity[components.size()-1];
      indexToEntity.pop_back();
      entityToIndex[backId]=index;
    }
  }
  
};
