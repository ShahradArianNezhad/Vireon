#include "engine/graphics/vertexArray/vertexArray.hpp"
#include <iostream>

VertexArray::VertexArray() { glGenVertexArrays(1, &ID); }

void VertexArray::useLayout(VertexLayout layout) {
  auto layoutArray = layout.getLayoutArray();
  for (int location = 0; location < layoutArray.size(); location++) {
    glVertexAttribPointer(location, layoutArray[location], GL_FLOAT, false,
                          layout.getStride(),
                          (void *)layout.getOffsetForLocation(location));
    glEnableVertexAttribArray(location);
  }
}

VertexArray::VertexArray(VertexArray &&other) noexcept : ID(other.ID) {
  other.ID = 0;
}
