#include "./engine.hpp"
#include "engine/meshManager/meshManager.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

void Engine::mainloop() {

  Scene scene;
  auto &e = makeRect(MeshManager::Color::White, 100, 100);
  scene.addEntity(&e);

  while (!window.windowShouldClose()) {
    renderer.flush();
    renderer.renderScene(scene);
    window.updateWindow();
  }
}

Entity &Engine::makeRect(MeshManager::Color color, float width, float height) {
  auto id = meshManager.makePrimitive(MeshManager::Primitive::Square, color);
  Entity &e = entityManager.newEntity();
  e.renderComp = RenderComponent(id, 0, true);
  e.transformComp = TransformComponent{.position = {400.0f, 100.0f, 0.0f},
                                       .scale = {width, height, 1.0f}};

  return e;
}
