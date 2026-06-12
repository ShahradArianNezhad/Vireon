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



class SpatialMap2{
  private:
    std::unordered_map<Cell, std::vector<EntityId>,Cell> map;
  public:
    float cellSize=32.0f;
    bool isNear(vec2 pos1,vec2 scale1,EntityId e2);

    void insert(EntityId id,vec2 pos,vec2 scale);
    std::vector<EntityId> getNearEntities(vec2 pos,vec2 scale);
    void clear(){map.clear();}
};



class SpatialMap{
  private:
    static constexpr float cellSize=3.5;
    static constexpr size_t gridSize=200;
    std::vector<EntityId> map[gridSize*gridSize];
  public:
    bool isNear(vec2 pos1,vec2 scale1,vec2 pos2,vec2 scale2);

    void insert(EntityId id,vec2 pos,vec2 scale);
    std::vector<EntityId>& getNearEntities(vec2 pos,vec2 scale);
    void clear(){for(auto& e:map)e.clear();}
};
