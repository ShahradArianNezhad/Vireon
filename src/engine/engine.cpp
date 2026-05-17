#include "./engine.hpp"
#include "engine/meshManager/meshManager.hpp"
#include <glm/ext/vector_float2.hpp>
#include <chrono>
#include <glm/ext/vector_float3.hpp>
#include "../game/game.hpp"


Engine::Engine(){
  SceneId defaultScene = newScene();
  renderer.useScene(defaultScene);
}



Entity *Engine::makeSprite(float x,float y,float width,float height,std::string spritePath){
  auto meshId = meshManager.makePrimitive(MeshManager::Primitive::SquareSprite);
  auto matId = materialManager.newMat();
  auto& matHandle = materialManager.get(matId);
  matHandle.setTexture(spritePath);
  Entity &e = entityManager.newEntity();
  e.renderComp = RenderComponent(meshId, matId);
  e.transformComp = TransformComponent{
      .position = {x, y, 0.0f},
      .scale = {width, height, 1.0f},
      .rotation=0
  };
  renderer.addEntity(&e);
  return &e;

}

Entity *Engine::makeRect(float x, float y, float width, float height) {
  auto meshId = meshManager.makePrimitive(MeshManager::Primitive::Square);
  auto matId = materialManager.newMat();
  auto& matHandle = materialManager.get(matId);
  matHandle.color=Color::White;
  Entity &e = entityManager.newEntity();
  e.renderComp = RenderComponent(meshId, matId);
  e.transformComp = TransformComponent{
      .position = {x, y, 0.0f},
      .scale = {width, height, 1.0f},
      .rotation=0
  };

  renderer.addEntity(&e);
  return &e;
}

Entity *Engine::makeCircle(float x, float y, float r) {
  auto meshId = meshManager.makePrimitive(MeshManager::Primitive::Circle);
  auto matId = materialManager.newMat();
  auto& matHandle = materialManager.get(matId);
  matHandle.color=Color::White;
  Entity &e = entityManager.newEntity();
  e.renderComp = RenderComponent(meshId, matId);
  e.transformComp = TransformComponent{
      .position = {x, y, 0.0f},
      .scale = {r, r, 1.0f},
      .rotation=0
  };

  renderer.addEntity(&e);
  return &e;
}


void Engine::syncFramerate(double dt){
    if(targetFPS > 0) {
    double targetFrameTime = 1.0 / targetFPS;
    if(dt < targetFrameTime) {
        clock.sleep(targetFrameTime - dt);  
      }
    }
}


void Engine::run(Game* game){
  game->init();
  while (!window.windowShouldClose()) {
    renderer.flush();
    renderer.renderCurrentScene();
    window.updateWindow();
    auto dt = clock.getDeltaTime();
    game->update(dt);
    syncFramerate(dt);
  }
}





