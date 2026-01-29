#include "renderer.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include "glad/gl.h"
#include <stdexcept>

void Renderer::flush() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::initGLAD() {
  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    throw std::runtime_error("ERROR: Failed to initialize glad");
  }
}
Renderer::Renderer(MeshManager &manager, MaterialManager &matManager)
    : meshManager(manager), materialManager(matManager) {
  initGLAD();
}

void Renderer::collectAndBatch(Scene &scene) {
  for (auto &entity : scene.collectEntities()) {
    batchManager.submit(entity);
  }
}

void Renderer::renderBatches() {
  for (auto &[key, batch] : batchManager.getBatches()) {
    auto mesh = meshManager.get(key.mesh);
    auto &mat = materialManager.get(key.material);
    shaderManager.useShader(mesh.layout);
    gpu.useMesh(mesh);
    mat.use();
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
  }
}

void Renderer::renderScene(Scene &scene) {
  collectAndBatch(scene);
  renderBatches();
  batchManager.cleanBatches();
}
