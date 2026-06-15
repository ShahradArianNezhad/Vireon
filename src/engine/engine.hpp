#pragma once
#include "../platform/window/GLFWwindow.hpp"
#include "./graphics/renderer/renderer.hpp"
#include "engine/entityManager/entityManager.hpp"
#include "engine/glyphManager/glyphManager.hpp"
#include "engine/materialManager/materialManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include "engine/sceneManager/sceneManager.hpp"
#include "engine/scheduleManager/scheduleManager.hpp"
#include "platform/input/inputHandler.hpp"
#include "utils/clock/clock.hpp"
#include "utils/spatialMap/spatialMap.hpp"
#include <cstdint>
#include <glm/ext/vector_float3.hpp>


struct Text{
  std::vector<EntityId> ids={};
  vec3 pos;
  std::string font;
  int size;
};


class Game;
class Engine {
private:
  MeshManager meshManager;
  MaterialManager materialManager;
  SceneManager sceneManager;
  GlyphManager glyphManager;
  Clock clock;
  uint32_t targetFPS=0;
  void buildSpatialMap();

  void syncFPS();

public:
  bool isNear(EntityId e1,EntityId e2);
  EngineWindow window{Screen::width, Screen::height,"myGame"};
  EntityManager entityManager;
  InputHandler inputHandler{window.getWindowPtr()};
  Renderer renderer{meshManager, materialManager,sceneManager,entityManager};
  void useCamera(EntityId camera,SceneId sceneid);


  Engine();
  ~Engine();
  SceneId newScene(){return sceneManager.newScene();}
  EntityId makeRect(vec3 pos,vec2 scale);
  EntityId makeCircle(vec3 pos, float r);
  EntityId makeSprite(vec3 pos,const std::string& spritePath,vec2 uvMin={0,0},vec2 uvMax={1,1});
  void changeSprite(EntityId id,const std::string& spritePath,vec2 uvMin={0,0},vec2 uvMax={1,1});
  void changeColor(EntityId id,uint32_t color);
  EntityId makeLight(vec2 pos,vec3 color, float radius,float intensity);
  void setAmbient(float a){renderer.ambient=a;}
  SceneId getCurrentScene(){return renderer.getCurrentScene();};
  bool isColliding(EntityId e1,EntityId e2);
  bool rectIsColliding(EntityId e1,EntityId e2);
  bool rectCircleIsColliding(EntityId e1,EntityId e2);
  bool circleIsColliding(EntityId e1,EntityId e2);
  void setTargetFPS(uint32_t t){targetFPS=t;};
  void run(Game* game);

  EntityId makeChar(char c,vec3 pos,std::string font="./assets/Arial.ttf",int size=64);
  Text makeText(std::string text,vec3 pos,std::string font,int size);
  void changeText(Text& text,const std::string& newText);
};



