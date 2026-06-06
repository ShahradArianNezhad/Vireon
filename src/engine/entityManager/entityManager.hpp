#pragma once
#include "engine/entityManager/ComponentManager/componentManager.hpp"
#include "utils/idManager/idManager.hpp"
#include "engine/entityManager/component/components.hpp"
#include "engine/eventManager/eventManager.hpp"
#include "utils/types.hpp"
#include <cstdint>
#include <vector>


using EntityId = uint32_t;

class EntityManager {
private:
  IDManager idManager;
  std::vector<EntityId> entities;

public:
  EntityManager(){};
  ComponentManager componentManager{};
  EntityId newEntity();
  EntityId newEntity(struct RenderComponent,struct TransformComponent);
  void deleteEntity(EntityId id);

  EntityId makeCamera();



  vec4 colorToVec4(EntityId id);
  void setColor(EntityId id,unsigned int hex);
  void setColor(EntityId id,unsigned int r,unsigned int g,unsigned int b,unsigned int a);
  void setPos(EntityId id,vec3 pos);
  vec3 getPos(EntityId id);
  mat4 makeModelMatrix(EntityId id);
};
