#pragma once
#include "engine/entityManager/ComponentManager/componentManager.hpp"
#include "utils/allocator/allocator.hpp"
#include "utils/idManager/idManager.hpp"
#include "engine/entityManager/component/components.hpp"
#include "utils/types.hpp"
#include <cstdint>
#include <vector>

struct EntityDestroyedEvent{
  EntityId id;
};

struct EntityCreatedEvent{
  EntityId id;
};

using EntityId = uint32_t;

class EntityManager {
private:
  IDManager idManager;
  EngineAllocator<EntityId> alloc{"Entity allocator"};
  std::vector<EntityId,EngineAllocator<EntityId>> entities{alloc};

public:
  EntityManager(){};
  ComponentManager componentManager{};
  EntityId newEntity();
  EntityId newEntity(struct Component::RENDER,struct Component::TRANSFORM);
  void deleteEntity(EntityId id);

  EntityId makeCamera();



  vec4 colorToVec4(EntityId id);
  void setColor(EntityId id,unsigned int hex);
  void setColor(EntityId id,unsigned int r,unsigned int g,unsigned int b,unsigned int a);
  void setPos(EntityId id,vec3 pos);
  vec3 getPos(EntityId id);
  mat4 makeModelMatrix(EntityId id);
};
