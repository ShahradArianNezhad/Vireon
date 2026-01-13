#include "engine/graphics/vertexLayout/vertexLayout.hpp"

int VertexLayout::getOffsetForLocation(unsigned int location) {
  int sum = 0;
  for (int i = 0; i < location; i++) {
    sum += layoutVector[i];
  }
  return sum * sizeof(float);
}

int VertexLayout::getStride() {
  int sum = 0;
  for (int i = 0; i < layoutVector.size(); i++) {
    sum += layoutVector[i];
  }
  return sum * sizeof(float);
}
