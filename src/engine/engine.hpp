#pragma once
#include "../platform/window/GLFWwindow.hpp"
#include "./graphics/renderer/renderer.hpp"
#include "engine/entityManager/entity/entity.hpp"
#include "engine/entityManager/entityManager.hpp"
#include "engine/materialManager/materialManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include "engine/sceneManager/sceneManager.hpp"

class Game;
class Engine {
private:
  EngineWindow window{Screen::width, Screen::height};
  MeshManager meshManager;
  MaterialManager materialManager;
  SceneManager sceneManager;
  EntityManager entityManager;

public:

  Renderer renderer{meshManager, materialManager,sceneManager};


  Engine() ;
  SceneId newScene(){return sceneManager.newScene();}
  Entity *makeRect(float x, float y, float width, float height);
  Entity *makeSprite(float x,float y,float width,float height,std::string spritePath);
  void run(Game& game);
};
