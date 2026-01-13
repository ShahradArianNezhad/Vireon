#pragma once
#include "engine/graphics/batch/batch.hpp"
#include "platform/window/GLFWwindow.hpp"
#include <vector>

class Renderer {
private:
  EngineWindow window{800, 600};
  std::vector<Batch> batches;
  std::vector<float> positions;

  void initGLAD();
  void draw(Batch &batch);
  void render();
  void clear();

public:
  Renderer();
  Renderer(const Renderer &) = delete;
  Renderer &operator=(const Renderer &) = delete;
  Renderer(Renderer &&other) = delete;

  void addObject(Object object);
  void mainloop();
};
