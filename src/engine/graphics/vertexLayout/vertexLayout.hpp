#pragma once
#include "engine/graphics/elementBuffer/elementBuffer.hpp"
#include "engine/graphics/vertexArray/vertexArray.hpp"
#include "engine/graphics/vertexBuffer/vertexBuffer.hpp"
#include <optional>

class VertexLayout {

public:
  std::optional<ElementBuffer> ebo;
  std::vector<VertexAttrib> attributes;
  int offset = 0;

  VertexArray vao;
  VertexLayout();
  VertexLayout(const void *indicies, size_t size);
  VertexLayout(const VertexLayout &) = delete;
  VertexLayout &operator=(const VertexLayout &) = delete;
  VertexLayout(VertexLayout &&other) noexcept;
  void pushFloat(int x, unsigned int location);
  void interpretVBO(VertexBuffer &vbo);
};
