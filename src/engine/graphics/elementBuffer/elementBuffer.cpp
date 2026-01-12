#include "engine/graphics/elementBuffer/elementBuffer.hpp"

ElementBuffer::ElementBuffer() { glGenBuffers(1, &ID); }

void ElementBuffer::setData(const void *indicies, size_t size) {
  bind();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indicies, GL_STATIC_DRAW);
  unbind();
  indexCount = size / sizeof(unsigned int);
}

ElementBuffer::ElementBuffer(ElementBuffer &&other) noexcept {
  ID = other.ID;
  indexCount = other.indexCount;
  other.indexCount = -1;
  other.ID = -1;
}

int ElementBuffer::getindexCount() {
  if (indexCount != -1) {
    return indexCount;
  } else {
    std::cout << "WARNING= getindexCount called with no indexCount value"
              << std::endl;
    return indexCount;
  }
}
