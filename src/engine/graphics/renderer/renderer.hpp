#pragma once
#include "engine/graphics/batch/batch.hpp"
#include "engine/graphics/shader/shader.hpp"
#include "engine/graphics/vertexArray/vertexArray.hpp"
#include "engine/graphics/vertexBuffer/vertexBuffer.hpp"
#include "platform/window/GLFWwindow.hpp"
#include <iostream>
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

  void submit(Batch &&batch) { batches.push_back(std::move(batch)); };
  void mainloop();
};
