#pragma once
#include "../platform/window/GLFWwindow.hpp"
#include "./graphics/renderer/renderer.hpp"
#include "engine/entityManager/entityManager.hpp"
#include "engine/materialManager/materialManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include "engine/sceneManager/sceneManager.hpp"
#include "engine/eventManager/eventManager.hpp"
#include "platform/input/inputHandler.hpp"
#include "utils/clock/clock.hpp"
#include <cstdint>
#include <glm/ext/vector_float3.hpp>


class Game;
class Engine {
private:
  EngineWindow window{Screen::width, Screen::height,"myGame"};
  MeshManager meshManager;
  MaterialManager materialManager;
  EventManager eventManager;
  SceneManager sceneManager{eventManager};
  Clock clock;
  uint32_t targetFPS=60;

  void syncFPS();

public:

  EntityManager entityManager{eventManager};
  InputHandler inputHandler{window.getWindowPtr()};
  Renderer renderer{meshManager, materialManager,sceneManager,entityManager};
  void useCamera(EntityId camera,SceneId sceneid);
  void sleep(int ms);


  Engine();
  ~Engine();
  SceneId newScene(){return sceneManager.newScene();}
  EntityId makeRect(float x, float y, float width, float height);
  EntityId makeCircle(float x, float y, float r);
  EntityId makeSprite(float x,float y,float width,float height,std::string spritePath);
  SceneId getCurrentScene(){return renderer.getCurrentScene();};
  void setTargetFPS(uint32_t t){targetFPS=t;};
  void run(Game* game);
};
