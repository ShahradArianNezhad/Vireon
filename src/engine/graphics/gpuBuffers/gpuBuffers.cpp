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
