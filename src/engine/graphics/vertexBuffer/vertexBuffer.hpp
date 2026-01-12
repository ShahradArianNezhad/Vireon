#pragma once
#include "glad/gl.h"
#include <cstddef>
#include <iostream>

class VertexBuffer {
private:
  unsigned int ID;

public:
  VertexBuffer();
  VertexBuffer(const void *data, size_t size);
  VertexBuffer(const VertexBuffer &) = delete;
  VertexBuffer &operator=(const VertexBuffer &) = delete;

  VertexBuffer(VertexBuffer &&other) noexcept;

  unsigned int getID() { return ID; }
  void bind() const { glBindBuffer(GL_ARRAY_BUFFER, ID); }
  void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
  void changeData(const void *data, size_t size, GLenum mode);
};

struct VertexAttrib {
  unsigned int index;
  int size;
  GLenum type;
  bool normalized;
  int stride;
  const void *offset;
};
