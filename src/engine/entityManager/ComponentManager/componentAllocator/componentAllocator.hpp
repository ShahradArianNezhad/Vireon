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
struct ComponentSlot{
  T comp;
  bool exists;
};


template <typename T>
class ComponentAllocator{

private:
  std::vector<ComponentSlot<T>> components;

public:

  T getComponent(EntityId id){
    return components.at(id).comp;
    LOG_WARN("getComponent:{} called on entity:{} which doesnt have that component",typeid(T).name(),id);
    return T{};
  }


  void setComponent(EntityId id,T& comp){
    try{
    auto& slot = components.at(id);
    slot.comp=comp;
    slot.exists=true;
    }catch(const std::out_of_range& ex){
      components.resize(id+100);
      auto& slot = components.at(id);
      slot.comp=comp;
      slot.exists=true;
    }
  }

  bool hasComponent(EntityId id){
    try{
    auto& slot = components.at(id);
    return slot.exists;
    }catch(const std::out_of_range& ex){
      return false;
    }

  };

  void deleteComponent(EntityId id){
    if(!hasComponent(id)){
      LOG_WARN("trying to delete non existing component:{} on entity:{}",typeid(T).name(),id);
      return;
    }
    auto& slot = components.at(id);
    slot.exists=false;
  }
  
};
