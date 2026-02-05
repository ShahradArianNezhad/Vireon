#include "engine/entityManager/entity/entity.hpp"
#include <vector>
class Scene {
private:
  std::vector<Entity *> entities;

public:
  std::vector<Entity *> &collectEntities() { return entities; }
  void addEntity(Entity *entity) { entities.push_back(entity); };
};
