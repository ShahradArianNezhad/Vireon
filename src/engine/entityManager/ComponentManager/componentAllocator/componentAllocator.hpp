#pragma once
#include "engine/entityManager/component/components.hpp"
#include "utils/allocator/allocator.hpp"
#include "utils/logger/logger.hpp"
#include <cstddef>
#include <format>

template <typename T>
struct ComponentSlot{
  T comp;
  bool exists;
};


template <typename T>
class ComponentAllocator{

private:
  EngineAllocator<ComponentSlot<T>> alloc{std::format("{} allocator",T{})};
  std::vector<ComponentSlot<T>,EngineAllocator<ComponentSlot<T>>> components{alloc};

public:


  T getComponent(EntityId id){
#ifndef ENGINE_OPTIMIZE
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
#ifndef ENGINE_DEBUG
    if(!hasComponent(id)){
      LOG_WARN("trying to delete non existing component:{} on entity:{}",typeid(T).name(),id);
      return;
    }
#endif
    auto& slot = components[id];
    slot.exists=false;
  }

  void tryDeleteComponent(EntityId id){
    if(!hasComponent(id))return;
    auto& slot = components[id];
    slot.exists=false;
  }
  
};
