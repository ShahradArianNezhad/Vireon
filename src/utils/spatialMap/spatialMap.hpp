#include "engine/entityManager/component/components.hpp"
#include "utils/types.hpp"
#include <cstddef>
#include <functional>
#include <unordered_map>
#include <vector>


struct Cell{
  int x;
  int y;
  bool operator==(const Cell&) const = default;

  size_t operator()(const Cell& c) const{
    return std::hash<int>()(c.x) ^ (std::hash<int>()(c.y) << 1);
  }
};



class SpatialMap{
  private:
    std::unordered_map<Cell, std::vector<EntityId>,Cell> map;
  public:
    size_t cellSize=64;

    void insert(EntityId id,vec2 pos,vec2 scale);
    std::vector<EntityId> getNearEntities(vec2 pos,vec2 scale);
    void clear(){map.clear();}
};
