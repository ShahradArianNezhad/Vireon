#pragma once
#include "../platform/window/GLFWwindow.hpp"
#include "./graphics/renderer/renderer.hpp"
#include "engine/entityManager/entity/entity.hpp"
#include "engine/entityManager/entityManager.hpp"
#include "engine/materialManager/materialManager.hpp"
#include "engine/meshManager/meshManager.hpp"

class Game;
class Engine {
private:
  EngineWindow window{Screen::width, Screen::height};
  MeshManager meshManager;
  MaterialManager materialManager;
  EntityManager entityManager;

public:
  Engine() {};
  Renderer renderer{meshManager, materialManager};
  Entity *makeRect(float x, float y, float width, float height, Color color);
  void run(Game& game);
};
