#pragma once
#include "engine/entityManager/component/components.hpp"
#include "./componentAllocator/componentAllocator.hpp"
#include <tuple>


using ComponentId = uint32_t;

class ComponentManager {
private:
  std::tuple<
  ComponentAllocator<TransformComponent>,
  ComponentAllocator<RenderComponent>,
  ComponentAllocator<CircleColliderComponent>,
  ComponentAllocator<RectColliderComponent>,
  ComponentAllocator<CameraComponent2D>,
  ComponentAllocator<UvRectComponent>,
  ComponentAllocator<LightComponent>
    > allocators;

public:
  template<ComponentType T>
    EnumToType<T>::type getComponent(EntityId id){
      auto allocator = std::get<ComponentAllocator<typename EnumToType<T>::type>>(allocators);
      return allocator.getComponent(id);
    }

  template<ComponentType T>
    void setComponent(EntityId id,typename EnumToType<T>::type comp){
      auto& allocator = std::get<ComponentAllocator<typename EnumToType<T>::type>>(allocators);
      allocator.setComponent(id,comp);
      LOG_DEBUG("set component:{} on entity: {}",comp,id);
    }


  template<ComponentType T>
    void deleteComponent(EntityId id){
      auto& allocator = std::get<ComponentAllocator<typename EnumToType<T>::type>>(allocators);
      allocator.deleteComponent(id);
      LOG_DEBUG("delete component:{} on entity: {}",T,id);
    }


  template<ComponentType... Ts>
    bool hasComponent(EntityId id){
      return (... && std::get<ComponentAllocator<typename EnumToType<Ts>::type>>(allocators)
          .hasComponent(id));
    };
  
};
