#pragma once
#include "engine/graphics/vertex/vertex.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include <vector>
class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  VertexLayout layout;
  Mesh() {};
  Mesh(std::vector<Vertex> aVertices, std::vector<unsigned int> aIndices,
       VertexLayout aLayout)
      : vertices(aVertices), indices(aIndices), layout(aLayout) {};
};
