#pragma once
#include "engine/entityManager/component/components.hpp"
#include <cstddef>


template <typename T>
class ComponentAllocator2{

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
    size_t lastIndex = components.size() - 1;

    if(index == lastIndex){
      components.pop_back();
      entityToIndex.erase(id);
      indexToEntity.pop_back();
    }else{
      components[index] = std::move(components[lastIndex]);
      components.pop_back();

      indexToEntity[index] = indexToEntity[lastIndex];
      indexToEntity.pop_back();

      entityToIndex[indexToEntity[index]] = index;
      entityToIndex.erase(id);
    }
  }
  
};



template <typename T>
class ComponentAllocator{

private:
  std::vector<T> components;
  std::unordered_map<EntityId,bool> EntityHasComponent;

public:

  T getComponent(EntityId id){
    if(EntityHasComponent[id])return components[id];
    LOG_WARN("getComponent:{} called on entity:{} which doesnt have that component",typeid(T).name(),id);
    return T{};
  }


  void setComponent(EntityId id,T& comp){
    if(EntityHasComponent[id]){
      components[id] = comp;
    }else{
      if(components.size()<=id)components.resize(id+100);
      components[id] = comp;
      EntityHasComponent[id]=true;
    }
  }

  bool hasComponent(EntityId id){return EntityHasComponent[id];};

  void deleteComponent(EntityId id){
    if(!EntityHasComponent[id]){
      LOG_WARN("trying to delete non existing component:{} on entity:{}",typeid(T).name(),id);
      return;
    }
    EntityHasComponent[id]=false;
  }
  
};
