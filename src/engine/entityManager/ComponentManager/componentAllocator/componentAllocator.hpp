#pragma once
#include "engine/entityManager/component/components.hpp"
#include "utils/allocator/allocator.hpp"
#include "utils/logger/logger.hpp"
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
  std::vector<ComponentSlot<T>,EngineAllocator<ComponentSlot<T>>> components;

public:


  T getComponent(EntityId id){
#ifdef ENGINE_DEBUG
    if(components.size()<id){
      LOG_WARN("getComponent:{} called on entity:{} which doesnt have that component",typeid(T).name(),id);
      return T{};
    }
#endif
      return components.at(id).comp;
  }


  void setComponent(EntityId id,T& comp){
    if(components.size()<=id)components.resize(id+100);
    auto& slot = components[id];
    slot.comp=comp;
    slot.exists=true;
  }

  bool hasComponent(EntityId id){
    if(components.size()<=id)return false;
    return components[id].exists;
  };

  void deleteComponent(EntityId id){
#ifdef ENGINE_DEBUG
    if(!hasComponent(id)){
      LOG_WARN("trying to delete non existing component:{} on entity:{}",typeid(T).name(),id);
      return;
    }
#endif
    auto& slot = components[id];
    slot.exists=false;
  }
  
};
