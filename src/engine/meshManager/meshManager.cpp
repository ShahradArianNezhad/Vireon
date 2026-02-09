#include "meshManager.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include "core/hasher64/hasher64.hpp"

MeshManager::MeshManager() {};

MeshID MeshManager::makePrimitive(Primitive shape, Color color) {
  float red = ((static_cast<unsigned int>(color) & 0xFF0000) >> 16);
  float green = ((static_cast<unsigned int>(color) & 0x00FF00) >> 8);
  float blue = (static_cast<unsigned int>(color) & 0x0000FF);
  red /= 255;
  green /= 255;
  blue /= 255;
  std::vector<Vertex> v;
  std::vector<unsigned int> i;

  switch (shape) {
    case Primitive::Triangle: 
      v = {
        Vertex{{0.5f, -0.5f, 0.0f}, {red, green, blue}},  //
        Vertex{{-0.5f, -0.5f, 0.0f}, {red, green, blue}}, //
        Vertex{{0.0f, 0.5f, 0.0f}, {red, green, blue}},   //
      };
      i = {0, 1, 2};
      break;


    case Primitive::Square: 
      v = {
        Vertex{{0.5f, -0.5f, 0.0f}, {red, green, blue}},  // bottom-right
        Vertex{{-0.5f, -0.5f, 0.0f}, {red, green, blue}}, // bottom-left
        Vertex{{0.5f, 0.5f, 0.0f}, {red, green, blue}},   // top-right
        Vertex{{-0.5f, 0.5f, 0.0f}, {red, green, blue}},  // top-left
      };
      i = {
        0, 1, 2, //
        3, 2, 1  //
      };  
      break;
  }
  Hasher64 hasher;
  hasher.combine(v.data(),v.size()*sizeof(Vertex));
  hasher.combine(i.data(),i.size()*sizeof(unsigned int));
  auto hash = hasher.digest();
  if(meshCache.contains(hash)){
    return meshCache[hash];
  }
  Mesh mesh{v, i, VertexLayout::PosColor};
  auto id= submit(mesh);
  meshCache[hash]=id;
  return id;
};
