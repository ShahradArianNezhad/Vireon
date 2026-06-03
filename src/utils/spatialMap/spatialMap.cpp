#include "./spatialMap.hpp"
#include <cmath>




void SpatialMap::insert(EntityId id,vec2 pos,vec2 scale){
  int minX= static_cast<int>(std::floor((pos.x-scale.x) /cellSize));
  int maxX= static_cast<int>(std::floor((pos.x+scale.x) /cellSize));
  int minY= static_cast<int>(std::floor((pos.y-scale.y) /cellSize));
  int maxY= static_cast<int>(std::floor((pos.y+scale.y) /cellSize));


  for(int i=minX;i<=maxX;i++){
    for(int j=minY;j<=maxY;j++){
      Cell cell{i,j};
      map[cell].push_back(id);
    }
  }

}

std::vector<EntityId> SpatialMap::getNearEntities(vec2 pos,vec2 scale){
  int minX= static_cast<int>(std::floor((pos.x-scale.x) /cellSize));
  int maxX= static_cast<int>(std::floor((pos.x+scale.x) /cellSize));
  int minY= static_cast<int>(std::floor((pos.y-scale.y) /cellSize));
  int maxY= static_cast<int>(std::floor((pos.y+scale.y) /cellSize));


  std::vector<EntityId> results;
  for(int i=minX;i<=maxX;i++){
    for(int j=minY;j<=maxY;j++){
      Cell cell{i,j};
      if(map.contains(cell)){
        auto& e=map[cell];
        for(auto id:e) results.push_back(id);
      }
    }
  }
  return results;
}

