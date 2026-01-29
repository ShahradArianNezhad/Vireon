#include "./elementBuffer.hpp"
#include <iostream>

ElementBuffer::ElementBuffer() { glGenBuffers(1, &ID); }

void ElementBuffer::upload(std::vector<unsigned int> &indices) {
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);
  indexCount = indices.size();
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
