#include "./engine.hpp"
#include "engine/entityManager/component/components.hpp"
#include "engine/glyphManager/glyphManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include <chrono>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <thread>
#include "../game/game.hpp"
#include "platform/window/GLFWwindow.hpp"
#include "utils/logger/logger.hpp"


Engine::Engine(){
  Logger::startLogger("logs.txt");
  Logger::setLogLevel(LogLevel::Debug);
  LOG_INFO("Engine initializing");
  SceneId defaultScene = newScene();
  renderer.useScene(defaultScene);
}

Engine::~Engine(){
  Logger::stopLogger();
}



EntityId Engine::makeSprite(vec3 pos,std::string spritePath,vec2 uvMin,vec2 uvMax){
  auto meshId = meshManager.makePrimitive(Primitive::SquareSprite);
  auto matId = materialManager.newMat(spritePath);
  vec2 spriteDims= materialManager.get(matId).getTextureDimensions();
  auto transformComp = TransformComponent{
      .position = {pos.x, pos.y, pos.z},
      .scale = {spriteDims.x*(uvMax.x-uvMin.x),spriteDims.y*(uvMax.y-uvMin.y), 1.0f},
      .rotation=0
  };
  auto id = entityManager.newEntity(RenderComponent{meshId,matId},transformComp);
  entityManager.componentManager.setComponent<ComponentType::UVRECT>(id, UvRectComponent{uvMin,uvMax});
  renderer.addEntity(id);
  LOG_DEBUG("Sprite entity created: mesh:{},mat:{},entityId:{}",meshId,matId,id);
  return id;

}

EntityId Engine::makeRect(vec3 pos,vec2 scale) {
  auto meshId = meshManager.makePrimitive(Primitive::Square);
  auto matId = materialManager.newMat();
  auto transformComp = TransformComponent{
      .position = {pos.x, pos.y, pos.z},
      .scale = {scale.x, scale.y, 1.0f},
  };

  auto id = entityManager.newEntity(RenderComponent{meshId,matId},transformComp);
  renderer.addEntity(id);
  LOG_DEBUG("rect entity created: mesh:{},mat:{},entityId:{}",meshId,matId,id);
  return id;
}

EntityId Engine::makeCircle(vec2 pos, float r) {
  auto meshId = meshManager.makePrimitive(Primitive::Circle);
  auto matId = materialManager.newMat();
  auto transformComp = TransformComponent{
      .position = {pos.x, pos.y, 0.0f},
      .scale = {r, r, 1.0f},
  };

  auto id = entityManager.newEntity(RenderComponent{meshId,matId},transformComp);
  renderer.addEntity(id);
  LOG_DEBUG("circle entity created: mesh:{},mat:{},entityId:{}",meshId,matId,id);
  return id;
}

void Engine::useCamera(EntityId camera,SceneId sceneid){
  auto scene = sceneManager.get(sceneid);
  if(scene!=nullptr) scene->setActiveCamera(camera);
  else std::cout << "error : cannot set camera on non existing scene" << std::endl;
  LOG_DEBUG("use camera id:{}",camera);
}



void Engine::syncFPS(){
  double dt = clock.getDeltaTime();
  double targetFrameTime = (targetFPS > 0) ? 1.0 / targetFPS : 0.0;
  if (targetFPS > 0 && dt < targetFrameTime) {
    clock.sleep(targetFrameTime - dt);
  }
}


void Engine::run(Game* game) {
    game->init();
    clock.reset();
    while (!window.windowShouldClose()) {
        double dt = clock.getDeltaTime();
        clock.setTimestamp();
        syncFPS();
        scheduleManager.update(dt);
        game->update(dt);
        renderer.renderCurrentScene();
        window.updateWindow();
    }
}


void Engine::sleep(int ms){
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}


EntityId Engine::makeChar(char c,vec3 pos,std::string font,int size){
  auto meshId = meshManager.makeQuad();
  auto texId = glyphManager.getGlyphTex(font, size);
  auto matId = materialManager.newMat(texId);
  auto& character = glyphManager.getGlyphChar(c,font,size);
  auto transformComp = TransformComponent{
      .position = {pos.x+character.width/2.0, pos.y-character.height/2.0, pos.z},
      .scale = {character.width/2.0, character.height/2.0, 1.0f},
      .rotation=0
  };
  auto uvComp = UvRectComponent{character.uvMin,character.uvMax};

  auto id = entityManager.newEntity(RenderComponent{meshId,matId},transformComp);
  entityManager.componentManager.setComponent<ComponentType::UVRECT>(id, uvComp);
  renderer.addEntity(id);
  return id;
}

std::vector<EntityId> Engine::makeText(std::string text,vec3 pos,std::string font,int size){
  std::vector<EntityId> ids;
  size_t advance=0;
  for(auto c:text){
    auto& character = glyphManager.getGlyphChar(c,font,size);
    auto id=makeChar(c,{pos.x+advance,pos.y,pos.z},font,size);
    ids.push_back(id);
    advance+=character.advance;
  }
  return ids;
}




EntityId Engine::makeLight(vec2 pos,vec3 color, float radius,float intensity) {
  auto transformComp = TransformComponent{
      .position = {pos.x, pos.y, 0.0f},
      .scale = {radius, radius, 1.0f},
  };
  auto lightComp = LightComponent{radius,intensity,color};

  auto id = entityManager.newEntity();
  entityManager.componentManager.setComponent<ComponentType::TRANSFORM>(id, transformComp);
  entityManager.componentManager.setComponent<ComponentType::LIGHT>(id, lightComp);
  renderer.addEntity(id);
  LOG_DEBUG("light entity created: pos:{},{},radius:{},entityId:{}",pos.x,pos.y,radius,id);
  return id;
}
