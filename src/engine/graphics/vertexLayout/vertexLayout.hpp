#pragma once
#include <vector>

class VertexLayout {
private:
  std::vector<unsigned int> layoutVector;

public:
  VertexLayout(std::vector<unsigned int> inputLayout)
      : layoutVector(inputLayout) {};
  VertexLayout() {};
  VertexLayout(VertexLayout &&other)
      : layoutVector(std::move(other.layoutVector)) {}

  bool operator==(const VertexLayout &other) {
    return this->layoutVector == other.layoutVector;
  }
  VertexLayout(const VertexLayout &other) : layoutVector(other.layoutVector) {}

  void pushFloat(unsigned int count) { layoutVector.push_back(count); };
  int getStride();
  std::vector<unsigned int> &getLayoutArray() { return layoutVector; };
  int getOffsetForLocation(unsigned int location);
};
