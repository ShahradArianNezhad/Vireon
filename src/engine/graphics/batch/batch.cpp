#include "batch.hpp"
#include <GL/gl.h>
#include <iostream>

Batch::Batch(Shader &&shader, VertexLayout &&mylayout)
    : shader(std::move(shader)), layout(std::move(mylayout)) {
  layout.interpretVBO(vbo);
}

Batch::Batch(Batch &&other) noexcept
    : shader(std::move(other.shader)), vbo(std::move(other.vbo)),
      texture(std::move(other.texture)),
      vertexData(std::move(other.vertexData)), changed(other.changed),
      layout(std::move(other.layout)) {}

void Batch::use() {
  if (changed) {
    updatedata();
    changed = false;
  }
  shader.use();
  layout.vao.bind();
  if (texture) {
    texture->bind();
  }
}

void Batch::addTexture(std::string imagePath) { texture.emplace(imagePath); }

void Batch::addVertex(std::initializer_list<float> floats) {
  if (floats.size() % (layout.offset / sizeof(float)) != 0) {
    std::cout << "inputted float in addVertex is not the size of the declared "
                 "layout size"
              << std::endl;
    return;
  }
  for (float f : floats) {
    vertexData.push_back(f);
  }
  changed = true;
}

void Batch::updatedata() {
  vbo.changeData(vertexData.data(), vertexData.size() * sizeof(float),
                 GL_DYNAMIC_DRAW);
}

int Batch::getIndexCount() {
  if (layout.ebo) {
    return layout.ebo->getindexCount();
  } else {
    return vertexData.size();
  }
}
