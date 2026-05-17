#include "./gpuBuffers.hpp"

void GpuBuffers::useMesh(Mesh &mesh) {
  vao.bind();
  vbo.bind();
  // dynamic modes?
  vbo.upload(mesh.vertices, GL_STATIC_DRAW);
  ebo.bind();
  ebo.upload(mesh.indices);
  vao.useLayout(mesh.layout);
}

void GpuBuffers::reset() {
  ebo.unbind();
  vbo.unbind();
  vao.unbind();
}


void GpuBuffers::useInstanceMat4(VertexBuffer& instanceVBO, unsigned int location){
  vao.bind();
  instanceVBO.bind();
  for (unsigned int i = 0; i < 4; i++) {
      glEnableVertexAttribArray(location + i);
      glVertexAttribPointer(location + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
      glVertexAttribDivisor(location + i, 1);
  }
}

void GpuBuffers::useInstanceVec4(VertexBuffer& instanceVBO, unsigned int location){
  vao.bind();
  instanceVBO.bind();
  glEnableVertexAttribArray(location);
  glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
  glVertexAttribDivisor(location, 1);
}
