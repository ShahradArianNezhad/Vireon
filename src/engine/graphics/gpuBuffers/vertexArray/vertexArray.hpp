#pragma once
#include "engine/meshManager/meshManager.hpp"
#include "glad/gl.h"

class VertexArray {
private:
  unsigned int ID;

public:
  VertexArray();
  ~VertexArray() { glDeleteBuffers(1, &ID); };
  VertexArray(const VertexArray &) = delete;
  VertexArray &operator=(const VertexArray &) = delete;
  VertexArray(VertexArray &&other) noexcept;

  void bind() const { glBindVertexArray(ID); }
  void unbind() const { glBindVertexArray(0); }
  void useLayout(VertexLayout layout);
};
