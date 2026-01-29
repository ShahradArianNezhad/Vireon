#pragma once
#include "glad/gl.h"
#include <cstddef>
#include <vector>

class ElementBuffer {
private:
  unsigned int ID;
  int indexCount = -1;

public:
  ElementBuffer();
  ~ElementBuffer() { glDeleteBuffers(1, &ID); };
  ElementBuffer(const ElementBuffer &) = delete;
  ElementBuffer &operator=(const ElementBuffer &) = delete;
  ElementBuffer(ElementBuffer &&other) noexcept;
  void upload(std::vector<unsigned int> &);
  void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
  void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
  int getindexCount();
};
