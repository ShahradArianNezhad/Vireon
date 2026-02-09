#include "entityManager.hpp"
#include "./entity/entity.hpp"

Entity *EntityManager::newEntity() {
  auto* e = new Entity;
  entities.push_back(e);
  return e;
}

