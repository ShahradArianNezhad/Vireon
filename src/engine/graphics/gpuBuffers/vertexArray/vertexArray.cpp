#include "./vertexArray.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include <iostream>
#include <stdexcept>

VertexArray::VertexArray() { glGenVertexArrays(1, &ID); }

void VertexArray::useLayout(VertexLayout layout) {
  if (layout == VertexLayout::Pos) {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  } else if (layout == VertexLayout::PosColor) {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
  }else if(layout==VertexLayout::PosUV){
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));

  }else{
    throw std::runtime_error{"VertexArray/useLayout: unsuppoerted layout"};
  }
}

VertexArray::VertexArray(VertexArray &&other) noexcept : ID(other.ID) {
  other.ID = 0;
}
