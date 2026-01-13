#pragma once
#include "engine/graphics/elementBuffer/elementBuffer.hpp"
#include "engine/graphics/vertexBuffer/vertexBuffer.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include "glad/gl.h"
#include <vector>

class VertexArray {
private:
  unsigned int ID;

public:
  VertexArray();
  VertexArray(const VertexArray &) = delete;
  VertexArray &operator=(const VertexArray &) = delete;
  VertexArray(VertexArray &&other) noexcept;

  void bind() const { glBindVertexArray(ID); }
  void unbind() const { glBindVertexArray(0); }
  void useLayout(VertexLayout layout);
};
