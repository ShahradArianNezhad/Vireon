#pragma once
#include "../platform/window/GLFWwindow.hpp"
#include "./graphics/renderer/renderer.hpp"
#include "engine/entityManager/entity/entity.hpp"
#include "engine/entityManager/entityManager.hpp"
#include "engine/materialManager/materialManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include "engine/sceneManager/sceneManager.hpp"
#include "core/clock/clock.hpp"
#include <cstdint>


class Game;
class Engine {
private:
  EngineWindow window{Screen::width, Screen::height};
  MeshManager meshManager;
  MaterialManager materialManager;
  SceneManager sceneManager;
  EntityManager entityManager;
  Clock clock;
  uint32_t targetFPS=0;

  void syncFramerate(double dt);

public:

  Renderer renderer{meshManager, materialManager,sceneManager};


  Engine();
  SceneId newScene(){return sceneManager.newScene();}
  Entity *makeRect(float x, float y, float width, float height);
  Entity *makeCircle(float x, float y, float r);
  Entity *makeSprite(float x,float y,float width,float height,std::string spritePath);
  SceneId getCurrentScene(){return renderer.getCurrentScene();};
  void setTargetFPS(uint32_t t){targetFPS=t;};
  void run(Game* game);
};
