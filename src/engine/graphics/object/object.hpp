#include "./objectData/objectData.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include <glm/fwd.hpp>

class Object {

private:
  unsigned int id;

public:
  Object(ObjectData inputData, VertexLayout inputLayout)
      : layout(inputLayout), data(inputData) {};
  Object(const Object &other) : layout(other.layout), data(other.data) {}
  unsigned int getId() { return id; };
  VertexLayout layout;
  ObjectData data;
};
