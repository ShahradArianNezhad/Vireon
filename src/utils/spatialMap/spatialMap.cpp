#include "./spatialMap.hpp"
#include <algorithm>
#include <cmath>




void SpatialMap::insert(EntityId id,vec2 pos,vec2 scale){
  int cellX = (int)((pos.x+scale.x)/cellSize);
  int cellY = (int)((pos.y+scale.y)/cellSize);
  cellX = std::clamp(cellX,0,(int)gridSize-1);
  cellY = std::clamp(cellY,0,(int)gridSize-1);
  map[cellX + cellY * gridSize].push_back(id);
}



using Entityid = uint32_t;
bool SpatialMap::isNear(vec2 pos1,vec2 scale1,vec2 pos2,vec2 scale2){
  int cellX1 = (int)((pos1.x+scale1.x)/cellSize);
  int cellY1 = (int)((pos1.y+scale1.y)/cellSize);
  cellX1 = std::clamp(cellX1,0,(int)gridSize-1);
  cellY1= std::clamp(cellY1,0,(int)gridSize-1);
  int cellX2 = (int)((pos2.x+scale2.x)/cellSize);
  int cellY2 = (int)((pos2.y+scale2.y)/cellSize);
  cellX2 = std::clamp(cellX2,0,(int)gridSize-1);
  cellY2 = std::clamp(cellY2,0,(int)gridSize-1);
  return (cellX1 + cellY1 * gridSize == cellX2 + cellY2 * gridSize );
}



std::vector<EntityId>& SpatialMap::getNearEntities(vec2 pos,vec2 scale){
  int cellX = (int)((pos.x+scale.x)/cellSize);
  int cellY = (int)((pos.y+scale.y)/cellSize);
  cellX = std::clamp(cellX,0,(int)gridSize-1);
  cellY = std::clamp(cellY,0,(int)gridSize-1);
  return map[cellX + cellY * gridSize];

}







