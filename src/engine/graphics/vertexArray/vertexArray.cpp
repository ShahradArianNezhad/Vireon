#include "engine/graphics/vertexArray/vertexArray.hpp"
#include "engine/graphics/elementBuffer/elementBuffer.hpp"
#include <utility>
#include <vector>

VertexArray::VertexArray() { glGenVertexArrays(1, &ID); }

void VertexArray::addBuffer(VertexAttrib attrib) {
  glVertexAttribPointer(attrib.index, attrib.size, attrib.type,
                        attrib.normalized, attrib.stride,
                        (void *)attrib.offset);
  glEnableVertexAttribArray(attrib.index);
}

VertexArray::VertexArray(VertexArray &&other) noexcept : ID(other.ID) {
  other.ID = 0;
}
