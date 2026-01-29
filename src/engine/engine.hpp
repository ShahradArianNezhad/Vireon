#pragma once
#include "../platform/window/GLFWwindow.hpp"
#include "./graphics/renderer/renderer.hpp"
#include "engine/materialManager/materialManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include "engine/scene/entity/entity.hpp"

class Engine {
private:
  EngineWindow window{800, 600};
  MeshManager meshManager;
  MaterialManager materialManager;

public:
  Renderer renderer{meshManager, materialManager};
  Engine() {};
  Entity makeTriangle();
  void mainloop();
};
