#include "./engine.hpp"
#include "engine/meshManager/meshManager.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include "../game/game.hpp"


Engine::Engine(){
  SceneId defaultScene = newScene();
  renderer.useScene(defaultScene);
}



Entity *Engine::makeRect(float x, float y, float width, float height, Color color) {
  auto id = meshManager.makePrimitive(MeshManager::Primitive::Square, color);
  Entity *e = entityManager.newEntity();
  e->renderComp = RenderComponent(id, 0);
  e->transformComp = TransformComponent{
      .position = {x, y, 0.0f},
      .scale = {width, height, 1.0f}
  };

  renderer.addEntity(e);
  return e;
}


void Engine::run(Game& game){
  game.init();
  while (!window.windowShouldClose()) {
    renderer.flush();
    renderer.renderCurrentScene();
    window.updateWindow();
    game.update(0);
  }
}
