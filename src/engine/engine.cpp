#include "./engine.hpp"
#include "engine/entityManager/component/components.hpp"
#include "engine/glyphManager/glyphManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include <chrono>
#include <cmath>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <thread>
#include "../game/game.hpp"
#include "platform/window/GLFWwindow.hpp"
#include "utils/logger/logger.hpp"
#include "audioManager/audioManager.hpp"


Engine::Engine(){
  std::string logFile="logs.txt";
  Logger::startLogger(logFile);
  Logger::setLogLevel(LogLevel::Debug);
  LOG_INFO("Engine initializing");
  SceneId defaultScene = newScene();
  renderer.useScene(defaultScene);
  AudioManager::init();
  EntityId camera=entityManager.makeCamera();
  useCamera(camera, getCurrentScene());
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

EntityId Engine::makeCircle(vec3 pos, float r) {
  auto meshId = meshManager.makePrimitive(Primitive::Circle);
  auto matId = materialManager.newMat();
  auto transformComp = TransformComponent{
      .position = {pos.x, pos.y, pos.z},
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
  LOG_INFO("set active camera : {}, on scene : {}",camera,sceneid);
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
        buildSpatialMap();
        double dt = clock.getDeltaTime();
        clock.setTimestamp();
        ScheduleManager::update(dt);
        game->update(dt);
        renderer.renderCurrentScene();
        window.updateWindow();
        spatialMap.clear();
        syncFPS();
    }
}


EntityId Engine::makeChar(char c,vec3 pos,std::string font,int size){
  auto meshId = meshManager.makeQuad();
  auto texId = glyphManager.getGlyphTex(font, size);
  auto matId = materialManager.newMat(texId);
  auto& character = glyphManager.getGlyphChar(c,font,size);
  auto transformComp = TransformComponent{
      .position = {pos.x+character.width/2.0, pos.y- character.bearingY+ (character.height)/2.0, pos.z},
      .scale = {character.width/2.0, character.height/2.0, 1.0f},
      .rotation=0
  };
  auto uvComp = UvRectComponent{character.uvMin,character.uvMax};

  auto id = entityManager.newEntity(RenderComponent{meshId,matId},transformComp);
  entityManager.componentManager.setComponent<ComponentType::UVRECT>(id, uvComp);
  renderer.addEntity(id);
  LOG_DEBUG("made character : {}, font : {}, size : {}",c,font,size);
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


void Engine::buildSpatialMap(){
  auto& entities = sceneManager.get(getCurrentScene())->collectEntities();
  for(auto e:entities){
    auto trans = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(e);
    spatialMap.insert(e, trans.position,  trans.scale);
  }
}


bool Engine::isColliding(EntityId e1,EntityId e2){
  if(!entityManager.componentManager.hasComponent<ComponentType::CIRCLECOLLIDER>(e1)&&
      !entityManager.componentManager.hasComponent<ComponentType::RECTCOLLIDER>(e1)){
    LOG_WARN("isCollding called on entity with no circleCollider of rectCollider component. id={}",e1);
    return false;
  }
  if(!entityManager.componentManager.hasComponent<ComponentType::CIRCLECOLLIDER>(e2)&&
      !entityManager.componentManager.hasComponent<ComponentType::RECTCOLLIDER>(e2)){
    LOG_WARN("isCollding called on entity with no circleCollider of rectCollider component. id={}",e2);
    return false;
  }

  if(entityManager.componentManager.hasComponent<ComponentType::CIRCLECOLLIDER>(e2)&&
      entityManager.componentManager.hasComponent<ComponentType::RECTCOLLIDER>(e1)) return rectCircleIsColliding(e1,e2);

  if(entityManager.componentManager.hasComponent<ComponentType::CIRCLECOLLIDER>(e1)&&
      entityManager.componentManager.hasComponent<ComponentType::RECTCOLLIDER>(e2)) return rectCircleIsColliding(e2,e1);

  if(entityManager.componentManager.hasComponent<ComponentType::CIRCLECOLLIDER>(e1)&&
      entityManager.componentManager.hasComponent<ComponentType::CIRCLECOLLIDER>(e2)) return circleIsColliding(e1,e2);

  if(entityManager.componentManager.hasComponent<ComponentType::RECTCOLLIDER>(e1)&&
      entityManager.componentManager.hasComponent<ComponentType::RECTCOLLIDER>(e2)) return rectIsColliding(e1,e2);
  return false;
}

bool Engine::isNear(EntityId e1,EntityId e2){
  auto trans = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(e1);
  auto nearEntites = spatialMap.getNearEntities({trans.position.x,trans.position.y}, trans.scale);
  for (auto e:nearEntites)if(e==e2)return true;
  return false;
}

bool Engine::circleIsColliding(EntityId e1,EntityId e2){
  if(!isNear(e1, e2))return false;
  auto t1 = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(e1);
  auto t2 = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(e2);
  auto r1 = entityManager.componentManager.getComponent<ComponentType::CIRCLECOLLIDER>(e1);
  auto r2 = entityManager.componentManager.getComponent<ComponentType::CIRCLECOLLIDER>(e2);
  float dx = (t1.position.x + r1.offset.x) - (t2.position.x + r2.offset.x);
  float dy = (t1.position.y + r1.offset.y) - (t2.position.y + r2.offset.y);
  float distSq = dx*dx + dy*dy;
  float radiusSum = r1.radius + r2.radius;

  return distSq <= radiusSum * radiusSum;
}

bool Engine::rectIsColliding(EntityId e1,EntityId e2){
  if(!isNear(e1, e2))return false;
  auto t1 = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(e1);
  auto t2 = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(e2);
  auto r1 = entityManager.componentManager.getComponent<ComponentType::RECTCOLLIDER>(e1);
  auto r2 = entityManager.componentManager.getComponent<ComponentType::RECTCOLLIDER>(e2);
  float left1   = t1.position.x + r1.offset.x - r1.scale.x/2;
  float right1  = t1.position.x + r1.offset.x + r1.scale.x/2;
  float top1    = t1.position.y + r1.offset.y - r1.scale.y/2;  
  float bottom1 = t1.position.y + r1.offset.y + r1.scale.y/2;
  float left2   = t2.position.x + r2.offset.x - r2.scale.x/2;
  float right2  = t2.position.x + r2.offset.x + r2.scale.x/2;
  float top2    = t2.position.y + r2.offset.y - r2.scale.y/2;
  float bottom2 = t2.position.y + r2.offset.y + r2.scale.y/2;

  return left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2;
}

bool Engine::rectCircleIsColliding(EntityId e1,EntityId e2){
  if(!isNear(e1, e2))return false;
  auto t1 = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(e1);
  auto t2 = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(e2);
  float closestX = std::max(t1.position.x - t1.scale.x * 0.5f,std::min(t2.position.x,t1.position.x + t1.scale.x * 0.5f));
  float closestY = std::max(t1.position.y - t1.scale.y * 0.5f,std::min(t2.position.y,t1.position.y + t1.scale.y * 0.5f));
  float dx = t2.position.x-closestX;
  float dy = t2.position.y-closestY;
  return dx * dx + dy * dy <= t2.scale.x*t2.scale.x;
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
