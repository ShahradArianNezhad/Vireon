#include "batch.hpp"
#include "engine/graphics/vertexArray/vertexArray.hpp"
#include <GL/gl.h>

Batch::Batch(Shader &&shader, VertexLayout mylayout)
    : shader(std::move(shader)), layout(mylayout) {
  vao.bind();
  vbo.bind();
  vao.useLayout(layout);
  vbo.unbind();
  vao.unbind();
}

void Batch::use() {
  if (changed) {
    updatedata();
    changed = false;
  }
  shader.use();
  vao.bind();
  if (texture) {
    texture->bind();
  }
}

void Batch::addTexture(std::string imagePath) { texture.emplace(imagePath); }

void Batch::updatedata() {
  std::vector<float> vertexData;
  for (Object obj : objects) {
    for (float data : obj.data.getVertexData()) {
      vertexData.push_back(data);
    }
  }
  vbo.bind();
  vbo.changeData(vertexData.data(), vertexData.size() * sizeof(float),
                 GL_DYNAMIC_DRAW);
  vbo.unbind();
}

void Batch::submit(Object obj) {
  objects.push_back(obj);
  changed = true;
};

int Batch::getIndicesCount() {
  if (isUsingEbo()) {
    return ebo->getindexCount();
  } else {
    int count = 0;
    for (auto &obj : objects) {
      for (int i = 0; i < obj.data.getDataCount(); i++) {
        count++;
      }
    }
    return count;
  }
}
