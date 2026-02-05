#pragma once
#include "../platform/window/GLFWwindow.hpp"
#include "./graphics/renderer/renderer.hpp"
#include "engine/entityManager/entity/entity.hpp"
#include "engine/entityManager/entityManager.hpp"
#include "engine/materialManager/materialManager.hpp"
#include "engine/meshManager/meshManager.hpp"

class Engine {
private:
  EngineWindow window{Screen::width, Screen::height};
  MeshManager meshManager;
  MaterialManager materialManager;
  EntityManager entityManager;

public:
  Renderer renderer{meshManager, materialManager};
  Engine() {};
  Entity &makeRect(MeshManager::Color color, float width, float height);
  void mainloop();
};
