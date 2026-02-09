#include "renderer.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include "glad/gl.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
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
  projectionMatrix = getProjectionMatrix();
}

void Renderer::collectAndBatch(Scene *scene) {
  for (auto &entity : scene->collectEntities()) {
    if (entity->renderComp)
      batchManager.submit(entity);
  }
}

glm::mat4 Renderer::getProjectionMatrix() {
  float w = static_cast<float>(Screen::width);
  float h = static_cast<float>(Screen::height);
  return glm::ortho(0.0f, w, h, 0.0f, -100.0f, 100.0f);
}

void Renderer::renderBatches() {

  for (auto &[key, batch] : batchManager.getBatches()) {
    auto &mesh = meshManager.get(key.mesh);
    auto &mat = materialManager.get(key.material);
    shaderManager.useShader(mesh.layout);
    auto &shader = shaderManager.getShaderHandle(mesh.layout);
    gpu.useMesh(mesh);
    mat.use();

    for (auto &e : batch) {
      auto transform = glm::mat4(1.0f);
      if (e->transformComp) {
        transform = glm::translate(transform, e->transformComp->position);
        transform = glm::scale(transform, e->transformComp->scale);
      }
      shader.setunifotmMat4("model", transform);
      shader.setunifotmMat4("projection", projectionMatrix);
      glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    }
  }
}

void Renderer::renderCurrentScene() {
  if (!currentScene) {
    std::cout << "warning: no scene used" << std::endl;
    return;
  }
  collectAndBatch(currentScene);
  renderBatches();
  batchManager.cleanBatches();
}
