#pragma once
#include "engine/graphics/vertex/vertex.hpp"
#include "glad/gl.h"
#include <cstddef>
#include <iostream>
#include <vector>

class VertexBuffer {
private:
  unsigned int ID;

public:
  VertexBuffer();
  VertexBuffer(std::vector<float> &data);
  ~VertexBuffer() { glDeleteBuffers(1, &ID); };
  VertexBuffer(const VertexBuffer &) = delete;
  VertexBuffer &operator=(const VertexBuffer &) = delete;

  VertexBuffer(VertexBuffer &&other) noexcept;

  unsigned int getID() { return ID; }
  void bind() const { glBindBuffer(GL_ARRAY_BUFFER, ID); }
  void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
  void upload(std::vector<Vertex> &data, GLenum mode);
  void upload(std::vector<float> &data, GLenum mode);
};
