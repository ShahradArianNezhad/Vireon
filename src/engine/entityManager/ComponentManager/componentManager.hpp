#pragma once
#include "engine/entityManager/component/components.hpp"
#include "./componentAllocator/componentAllocator.hpp"
#include "engine/eventManager/eventManager.hpp"
#include <tuple>


using ComponentId = uint32_t;

template<typename T>
struct ComponentChangingEvent{
  EntityId entity;
  T newComp;
};

class ComponentManager {
private:
  std::tuple<
  ComponentAllocator<Component::TRANSFORM>,
  ComponentAllocator<Component::RENDER>,
  ComponentAllocator<Component::CIRCLECOLLIDER>,
  ComponentAllocator<Component::RECTCOLLIDER>,
  ComponentAllocator<Component::CAMERA2D>,
  ComponentAllocator<Component::UVRECT>,
  ComponentAllocator<Component::LIGHT>
    > allocators;

public:
  template<typename T>
    [[nodiscard]] T getComponent(EntityId id){
      auto& allocator = std::get<ComponentAllocator<T>>(allocators);
      return allocator.getComponent(id);
    }

  template<typename T>
    void setComponent(EntityId id,T comp){
      auto& allocator = std::get<ComponentAllocator<T>>(allocators);
      EventManager::emit(ComponentChangingEvent<T>{id,comp});
      allocator.setComponent(id,comp);
#ifdef DEBUG_VERBOSE
      LOG_DEBUG("set component:{} on entity: {}",comp,id);
#endif
    }


  template<typename T>
    void deleteComponent(EntityId id){
      auto& allocator = std::get<ComponentAllocator<T>>(allocators);
      allocator.deleteComponent(id);
#ifdef DEBUG_VERBOSE
      LOG_DEBUG("delete component:{} on entity: {}",T,id);
#endif
    }

    void deleteAllComponents(EntityId id){
      std::apply([&](auto&... allocator) {
          (allocator.tryDeleteComponent(id), ...);
          }, allocators);

#ifdef DEBUG_VERBOSE
      LOG_DEBUG("delete all components on entity: {}",T,id);
#endif
    }


  template<typename... Ts>
    bool hasComponent(EntityId id){
      return (... && std::get<ComponentAllocator<Ts>>(allocators)
          .hasComponent(id));
    };
  
};
