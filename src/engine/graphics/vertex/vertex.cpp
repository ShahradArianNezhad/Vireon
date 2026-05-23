#include "./vertex.hpp"
#include <iostream>

std::vector<float> Vertex::getVertexData() const {
  std::vector<float> vertex;
  addPosition(vertex);
  addColor(vertex);
  addTextureCoords(vertex);
  return vertex;
}

void Vertex::addPosition(std::vector<float> &aVertex) const {
  for (int i = 0; i < 3; i++) {
    aVertex.push_back(position[i]);
  }
}

void Vertex::addColor(std::vector<float> &aVertex) const {
  if (color) {
    for (int i = 0; i < 3; i++) {
      aVertex.push_back((*color)[i]);
    }
  }
}

void Vertex::addTextureCoords(std::vector<float> &aVertex) const {
  if (textureCoords) {
    for (int i = 0; i < 2; i++) {
      aVertex.push_back((*textureCoords)[i]);
    }
  }
}

std::vector<float> Vertex::mergeVertices(std::vector<Vertex> vertices) {
  std::vector<float> floatVertices;
  for (auto &vertex : vertices) {
    for (auto &vNumber : vertex.getVertexData()) {
      floatVertices.push_back(vNumber);
    }
  }
  return floatVertices;
}
