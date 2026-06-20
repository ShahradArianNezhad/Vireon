#pragma once
#include "../platform/window/GLFWwindow.hpp"
#include "./graphics/renderer/renderer.hpp"
#include "engine/entityManager/entityManager.hpp"
#include "engine/glyphManager/glyphManager.hpp"
#include "engine/materialManager/materialManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include "engine/sceneManager/sceneManager.hpp"
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
  Layer layer;
};


class Game;
class Engine {
private:
  Logger logger;
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
  void useCamera(EntityId camera,SceneId sceneid);


  Engine();
  ~Engine();
  SceneId newScene();
  EntityId makeRect(vec3 pos,vec2 scale,Layer layer=Layer::WORLD);
  EntityId makeCircle(vec3 pos, float r,Layer layer=Layer::WORLD);
  EntityId makeSprite(vec3 pos,const std::string& spritePath,vec2 uvMin={0,0},vec2 uvMax={1,1},Layer layer=Layer::WORLD);
  void changeSprite(EntityId id,const std::string& spritePath,vec2 uvMin={0,0},vec2 uvMax={1,1});
  void changeColor(EntityId id,uint32_t color);
  void setVisibility(EntityId id,bool value);
  EntityId makeLight(vec2 pos,vec3 color, float radius,float intensity,Layer layer=Layer::WORLD);
  void setAmbient(float a);
  SceneId getCurrentScene();
  bool isColliding(EntityId e1,EntityId e2);
  bool rectIsColliding(EntityId e1,EntityId e2);
  bool rectCircleIsColliding(EntityId e1,EntityId e2);
  bool circleIsColliding(EntityId e1,EntityId e2);
  EntityId getActiveCamera();
  void setTargetFPS(uint32_t t);
  void run(Game* game);

  EntityId makeChar(char c,vec3 pos,int size=50,Layer layer=Layer::WORLD,const std::string& font="fonts/Arial.ttf");
  Text makeText(const std::string& text,vec3 pos,int size=50,Layer layer=Layer::WORLD,const std::string& font="fonts/Atial.ttf");
  void changeText(Text& text,const std::string& newText);
private:
  Renderer renderer{meshManager, materialManager,sceneManager,entityManager};
};



