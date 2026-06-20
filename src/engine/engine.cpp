#include "./engine.hpp"
#include "engine/entityManager/component/components.hpp"
#include "engine/glyphManager/glyphManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include <glm/ext/vector_float2.hpp>
#include "engine/scheduleManager/scheduleManager.hpp"
#include <glm/ext/vector_float3.hpp>
#include "../game/game.hpp"
#include "platform/window/GLFWwindow.hpp"
#include "utils/file/fileIO.hpp"
#include "utils/logger/logger.hpp"
#include "audioManager/audioManager.hpp"


Engine::Engine(){
  FileIO::findBaseDir();
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



EntityId Engine::makeSprite(vec3 pos,const std::string& spritePath,vec2 uvMin,vec2 uvMax,Layer layer){
  auto meshId = meshManager.makePrimitive(Primitive::SquareSprite);
  auto matId = materialManager.newMat(spritePath);
  vec2 spriteDims= materialManager.get(matId).getTextureDimensions();
  auto transformComp = Component::TRANSFORM{
      .position = {pos.x, pos.y, pos.z},
      .scale = {spriteDims.x*(uvMax.x-uvMin.x),spriteDims.y*(uvMax.y-uvMin.y), 1.0f},
      .rotation=0
  };
  auto id = entityManager.newEntity(Component::RENDER{meshId,matId},transformComp);
  entityManager.componentManager.setComponent<Component::UVRECT>(id, Component::UVRECT{uvMin,uvMax});
  renderer.addEntity(id,layer);
  LOG_DEBUG("Sprite entity created: mesh:{},mat:{},entityId:{}",meshId,matId,id);
  return id;
}

void Engine::changeSprite(EntityId id,const std::string& spritePath,vec2 uvMin,vec2 uvMax){
  auto matId = materialManager.newMat(spritePath);
  auto renderComp = entityManager.componentManager.getComponent<Component::RENDER>(id);
  renderComp.material=matId;
  entityManager.componentManager.setComponent(id, renderComp);
  vec2 spriteDims= materialManager.get(matId).getTextureDimensions();
  auto transformComp = entityManager.componentManager.getComponent<Component::TRANSFORM>(id);
  transformComp.scale = {spriteDims.x*(uvMax.x-uvMin.x),spriteDims.y*(uvMax.y-uvMin.y), 1.0f};
  entityManager.componentManager.setComponent(id, transformComp);
  entityManager.componentManager.setComponent<Component::UVRECT>(id, Component::UVRECT{uvMin,uvMax});
  LOG_DEBUG("Sprite changed on entity : {}, to : {}",id,spritePath);
}

void Engine::changeColor(EntityId id,uint32_t color){
  auto renderComp = entityManager.componentManager.getComponent<Component::RENDER>(id);
  renderComp.color=color;
  entityManager.componentManager.setComponent(id, renderComp);
}

void Engine::setVisibility(EntityId id,bool value){
  auto renderComp = entityManager.componentManager.getComponent<Component::RENDER>(id);
  renderComp.visible=value;
  entityManager.componentManager.setComponent(id, renderComp);
}



EntityId Engine::makeRect(vec3 pos,vec2 scale,Layer layer) {
  auto meshId = meshManager.makePrimitive(Primitive::Square);
  auto matId = materialManager.newMat();
  auto transformComp = Component::TRANSFORM{
      .position = {pos.x, pos.y, pos.z},
      .scale = {scale.x, scale.y, 1.0f},
  };

  auto id = entityManager.newEntity(Component::RENDER{meshId,matId},transformComp);
  renderer.addEntity(id,layer);
  LOG_DEBUG("rect entity created: mesh:{},mat:{},entityId:{}",meshId,matId,id);
  return id;
}

EntityId Engine::makeCircle(vec3 pos, float r,Layer layer) {
  auto meshId = meshManager.makePrimitive(Primitive::Circle);
  auto matId = materialManager.newMat();
  auto transformComp = Component::TRANSFORM{
      .position = {pos.x, pos.y, pos.z},
      .scale = {r, r, 1.0f},
  };
  auto id = entityManager.newEntity(Component::RENDER{meshId,matId},transformComp);
  renderer.addEntity(id,layer);
  LOG_DEBUG("circle entity created: mesh:{},mat:{},entityId:{}",meshId,matId,id);
  return id;
}

void Engine::useCamera(EntityId camera,SceneId sceneid){
  auto scene = sceneManager.get(sceneid);
  if(scene!=nullptr) scene->setActiveCamera(camera);
  else LOG_ERROR("cannot set camera on non existing scene");
  LOG_DEBUG("use camera id:{}",camera);
  LOG_INFO("set active camera : {}, on scene : {}",camera,sceneid);
}


EntityId Engine::getActiveCamera(){
  auto scene = sceneManager.get(getCurrentScene());
  if(scene!=nullptr) return scene->getActiveCamera();
  else{
    LOG_ERROR("cannot get active camera on non existing scene");
    return UINT32_MAX;
  }

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
        ScheduleManager::update(dt);
        game->update(dt);
        renderer.renderCurrentScene();
        window.updateWindow();
        syncFPS();
    }
}


EntityId Engine::makeChar(char c,vec3 pos,int size,Layer layer,const std::string& font){
  auto meshId = meshManager.makeQuad();
  auto texId = glyphManager.getGlyphTex(font, size);
  auto matId = materialManager.newMat(texId);
  auto& character = glyphManager.getGlyphChar(c,font,size);
  auto transformComp = Component::TRANSFORM{
      .position = {pos.x+character.width/2.0, pos.y- character.bearingY+ (character.height)/2.0, pos.z},
      .scale = {character.width/2.0, character.height/2.0, 1.0f},
      .rotation=0
  };
  auto uvComp = Component::UVRECT{character.uvMin,character.uvMax};

  auto id = entityManager.newEntity(Component::RENDER{meshId,matId},transformComp);
  entityManager.componentManager.setComponent<Component::UVRECT>(id, uvComp);
  renderer.addEntity(id,layer);
  return id;
}

Text Engine::makeText(const std::string& text,vec3 pos,int size,Layer layer,const std::string& font){
  Text t{.pos=pos,.font=font,.size=size,.layer=layer};
  size_t advance=0;
  for(auto c:text){
    auto& character = glyphManager.getGlyphChar(c,font,size);
    auto id=makeChar(c,{pos.x+advance,pos.y,pos.z},size,layer,font);
    t.ids.push_back(id);
    advance+=character.advance;
  }
  LOG_DEBUG("made text : {}, font : {}, size : {}",text,font,size);
  return t;
}




bool Engine::isColliding(EntityId e1,EntityId e2){

#ifdef ENGINE_DEBUG
  if(!entityManager.componentManager.hasComponent<Component::CIRCLECOLLIDER>(e1)&&
      !entityManager.componentManager.hasComponent<Component::RECTCOLLIDER>(e1)){
    LOG_WARN("isCollding called on entity with no circleCollider of rectCollider component. id={}",e1);
    return false;
  }
  if(!entityManager.componentManager.hasComponent<Component::CIRCLECOLLIDER>(e2)&&
      !entityManager.componentManager.hasComponent<Component::RECTCOLLIDER>(e2)){
    LOG_WARN("isCollding called on entity with no circleCollider of rectCollider component. id={}",e2);
    return false;
  }
#endif

  if(entityManager.componentManager.hasComponent<Component::CIRCLECOLLIDER>(e2)&&
      entityManager.componentManager.hasComponent<Component::RECTCOLLIDER>(e1)) return rectCircleIsColliding(e1,e2);

  if(entityManager.componentManager.hasComponent<Component::CIRCLECOLLIDER>(e1)&&
      entityManager.componentManager.hasComponent<Component::RECTCOLLIDER>(e2)) return rectCircleIsColliding(e2,e1);

  if(entityManager.componentManager.hasComponent<Component::CIRCLECOLLIDER>(e1)&&
      entityManager.componentManager.hasComponent<Component::CIRCLECOLLIDER>(e2)) return circleIsColliding(e1,e2);

  if(entityManager.componentManager.hasComponent<Component::RECTCOLLIDER>(e1)&&
      entityManager.componentManager.hasComponent<Component::RECTCOLLIDER>(e2)) return rectIsColliding(e1,e2);
  return false;
}


bool Engine::circleIsColliding(EntityId e1,EntityId e2){
  auto t1 = entityManager.componentManager.getComponent<Component::TRANSFORM>(e1);
  auto t2 = entityManager.componentManager.getComponent<Component::TRANSFORM>(e2);
  auto r1 = entityManager.componentManager.getComponent<Component::CIRCLECOLLIDER>(e1);
  auto r2 = entityManager.componentManager.getComponent<Component::CIRCLECOLLIDER>(e2);
  float dx = (t1.position.x + r1.offset.x) - (t2.position.x + r2.offset.x);
  float dy = (t1.position.y + r1.offset.y) - (t2.position.y + r2.offset.y);
  float distSq = dx*dx + dy*dy;
  float radiusSum = r1.radius + r2.radius;

  return distSq <= radiusSum * radiusSum;
}

bool Engine::rectIsColliding(EntityId e1,EntityId e2){
  auto t1 = entityManager.componentManager.getComponent<Component::TRANSFORM>(e1);
  auto t2 = entityManager.componentManager.getComponent<Component::TRANSFORM>(e2);
  auto r1 = entityManager.componentManager.getComponent<Component::RECTCOLLIDER>(e1);
  auto r2 = entityManager.componentManager.getComponent<Component::RECTCOLLIDER>(e2);
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
  auto t1 = entityManager.componentManager.getComponent<Component::TRANSFORM>(e1);
  auto t2 = entityManager.componentManager.getComponent<Component::TRANSFORM>(e2);
  float closestX = std::max(t1.position.x - t1.scale.x * 0.5f,std::min(t2.position.x,t1.position.x + t1.scale.x * 0.5f));
  float closestY = std::max(t1.position.y - t1.scale.y * 0.5f,std::min(t2.position.y,t1.position.y + t1.scale.y * 0.5f));
  float dx = t2.position.x-closestX;
  float dy = t2.position.y-closestY;
  return dx * dx + dy * dy <= t2.scale.x*t2.scale.x;
}


EntityId Engine::makeLight(vec2 pos,vec3 color, float radius,float intensity,Layer layer) {
  auto transformComp = Component::TRANSFORM{
      .position = {pos.x, pos.y, 0.0f},
      .scale = {radius, radius, 1.0f},
  };
  auto lightComp = Component::LIGHT{radius,intensity,color};

  auto id = entityManager.newEntity();
  entityManager.componentManager.setComponent<Component::TRANSFORM>(id, transformComp);
  entityManager.componentManager.setComponent<Component::LIGHT>(id, lightComp);

  renderer.addEntity(id,layer);
  LOG_DEBUG("light entity created: pos:{},{},radius:{},entityId:{}",pos.x,pos.y,radius,id);
  return id;
}



void Engine::changeText(Text& text,const std::string& newText){
  if(text.ids.empty()){
    LOG_ERROR("empty text vector passed changeText");
    return;
  }
  while(!text.ids.empty()){
    entityManager.deleteEntity(text.ids.back());
    text.ids.pop_back();
  }
  LOG_DEBUG("changed a text to : {}",newText);
  text = makeText(newText, text.pos, text.size,text.layer, text.font);
}



SceneId Engine::newScene(){return sceneManager.newScene();}

void Engine::setTargetFPS(uint32_t t){targetFPS=t;};

void Engine::setAmbient(float a){renderer.ambient=a;}
SceneId Engine::getCurrentScene(){return renderer.getCurrentScene();};
