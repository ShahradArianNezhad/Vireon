#include "meshManager.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include "core/hasher64/hasher64.hpp"

MeshManager::MeshManager() {};

MeshID MeshManager::makePrimitive(Primitive shape) {
  std::vector<Vertex> v;
  std::vector<unsigned int> i;

  switch (shape) {
    case Primitive::Triangle: 
      v = {
        Vertex{{0.5f, -0.5f, 0.0f}, },  //
        Vertex{{-0.5f, -0.5f, 0.0f}, }, //
        Vertex{{0.0f, 0.5f, 0.0f}, },   //
      };
      i = {0, 1, 2};
      break;


    case Primitive::Square: 
      v = {
        Vertex{{0.5f, -0.5f, 0.0f}, },  // bottom-right
        Vertex{{-0.5f, -0.5f, 0.0f}, }, // bottom-left
        Vertex{{0.5f, 0.5f, 0.0f}, },   // top-right
        Vertex{{-0.5f, 0.5f, 0.0f}, },  // top-left
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
  Mesh mesh{v, i, VertexLayout::Pos};
  auto id= submit(mesh);
  meshCache[hash]=id;
  return id;
};
