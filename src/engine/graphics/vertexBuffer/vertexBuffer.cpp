#include "engine/graphics/vertexBuffer/vertexBuffer.hpp"

VertexBuffer::VertexBuffer(const void *data, size_t size) {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ARRAY_BUFFER, ID);
  changeData(data, size, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer() { glGenBuffers(1, &ID); }

void VertexBuffer::changeData(const void *data, size_t size, GLenum mode) {
  glBufferData(GL_ARRAY_BUFFER, size, data, mode);
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept {
  ID = other.ID;
  other.ID = 0;
}
