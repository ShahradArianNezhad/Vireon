#include "./entity/entity.hpp"
#include <vector>
class EntityManager {
private:
  std::vector<Entity> entities;

public:
  Entity& newEntity();
};
