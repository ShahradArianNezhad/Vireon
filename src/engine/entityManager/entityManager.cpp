#include "entityManager.hpp"
#include "./entity/entity.hpp"

Entity& EntityManager::newEntity() {
  entities.push_back(Entity{});
  return entities.back();
}

