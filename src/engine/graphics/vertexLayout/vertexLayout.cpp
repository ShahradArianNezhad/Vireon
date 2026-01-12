#include "vertexLayout.hpp"
#include "engine/graphics/vertexBuffer/vertexBuffer.hpp"
#include <iostream>

VertexLayout::VertexLayout() {};

VertexLayout::VertexLayout(const void *indicies, size_t size)
    : ebo(ElementBuffer()) {
  vao.bind();
  ebo->setData(indicies, size);
  vao.unbind();
}

void VertexLayout::pushFloat(int count, unsigned int location) {
  VertexAttrib attrib;
  attrib.index = location;
  attrib.size = count;
  attrib.type = GL_FLOAT;
  attrib.normalized = false;
  attrib.stride = 0;
  attrib.offset = reinterpret_cast<void *>(static_cast<uintptr_t>(offset));
  attributes.push_back(attrib);
  offset += count * sizeof(float);
}

void VertexLayout::interpretVBO(VertexBuffer &vbo) {
  vao.bind();
  if (ebo) {
    ebo->bind();
  }
  vbo.bind();
  for (auto &attrib : attributes) {
    attrib.stride = offset;
    vao.addBuffer(attrib);
  }
  vbo.unbind();
  vao.unbind();
}

VertexLayout::VertexLayout(VertexLayout &&other) noexcept
    : ebo(std::move(other.ebo)), attributes(std::move(other.attributes)),
      offset(other.offset), vao(std::move(other.vao)) {}
