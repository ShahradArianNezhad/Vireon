#include "meshManager.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"

#define CIRCLE_V_COUNT 100

MeshManager::MeshManager() {};

MeshID MeshManager::makePrimitive(Primitive shape) {
  Hasher64 hasher;
  hasher.combine(&shape, 4);
  auto sig = hasher.digest();
  if(meshCache.contains(sig)){return meshCache[sig];}
  std::vector<Vertex> v;
  std::vector<unsigned int> i;
  VertexLayout layout{0};

  switch (shape) {
    case Primitive::Triangle: 
      v = {
        Vertex{{0.5f, -0.5f, 0.0f}, },  //
        Vertex{{-0.5f, -0.5f, 0.0f}, }, //
        Vertex{{0.0f, 0.5f, 0.0f}, },   //
      };
      i = {0, 1, 2};
      layout = VertexLayout::Pos;
      break;


    case Primitive::Square: 
      v = {
        Vertex{{1.0f, -1.0f, 0.0f}, },  // bottom-right
        Vertex{{-1.0f, -1.0f, 0.0f}, }, // bottom-left
        Vertex{{1.0f, 1.0f, 0.0f}, },   // top-right
        Vertex{{-1.0f, 1.0f, 0.0f}, },  // top-left
      };
      i = {
        0, 1, 2, //
        3, 2, 1  //
      };
      layout = VertexLayout::Pos;
      break;

    case Primitive::Circle: 
      v = {Vertex{{0.0f, 0.0f, 0.0f}}};
      
      for (int k = 0; k <= CIRCLE_V_COUNT; ++k) {
        float theta = 2.0f * M_PI * k / CIRCLE_V_COUNT;
        v.push_back(Vertex({ std::cos(theta), std::sin(theta), 0.0f }));
      }

      for(size_t j=2;j<v.size();j++){
        i.push_back(0);
        i.push_back(j-1);
        i.push_back(j);
      }

      layout = VertexLayout::Pos;
      break;

    case Primitive::SquareSprite: 
      v = {
        Vertex{{1, -1, 0.0f},{1.0,0.0f} },  // bottom-right
        Vertex{{-1, -1, 0.0f},{0.0f,0.0f} }, // bottom-left
        Vertex{{1, 1, 0.0f},{1.0,1.0f} },   // top-right
        Vertex{{-1, 1, 0.0f},{0.0f,1.0f} },  // top-left
      };
      i = {
        0, 1, 2, //
        3, 2, 1  //
      };
      layout = VertexLayout::PosUV;
      break;
  }
  Mesh mesh{v, i, layout};
  auto id= submit(mesh);
  meshCache[sig]=id;
#ifdef DEBUG_VERBOSE
  LOG_DEBUG("makePrimitive called: shape={}",shape);
#endif
  return id;
};



MeshID MeshManager::makeQuad(){
  Hasher64 hasher;
  hasher.combine("quad", 4);
  auto sig = hasher.digest();
  if(meshCache.contains(sig)){return meshCache[sig];}

  std::vector<Vertex> v = {
    Vertex{ {-1,1,0},     {0.0f, 1.0f} }, // tl
    Vertex{ {-1,-1,0},       {0.0f, 0.0f} }, // bl
    Vertex{ {1,-1,0},     {1.0f, 0.0f} }, // br
    Vertex{ {1,1,0},   {1.0f, 1.0f} }}; // tr
    VertexLayout layout = VertexLayout::PosUVText;
    std::vector<unsigned int>i = {
      0, 1, 2, 
      0, 2, 3  
    };
    Mesh mesh{v, i, layout};

    auto id= submit(mesh);
    meshCache[sig]=id;
    return id;
}
