#include "./vertexBuffer.hpp"
#include "engine/graphics/vertex/vertex.hpp"
#include "glad/gl.h"
#include <vector>

VertexBuffer::VertexBuffer(std::vector<float> &data) {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ARRAY_BUFFER, ID);
  upload(data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer() { glGenBuffers(1, &ID); }

void VertexBuffer::upload(std::vector<float> &data, GLenum mode) {
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), mode);
}
void VertexBuffer::upload(std::vector<Vertex> &data, GLenum mode) {
  auto merged = Vertex::mergeVertices(data);
  glBufferData(GL_ARRAY_BUFFER, merged.size() * sizeof(float), merged.data(),
               mode);
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept {
  ID = other.ID;
  other.ID = 0;
}
