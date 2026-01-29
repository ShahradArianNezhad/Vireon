#pragma once
#define GLFW_INCLUDE_NONE
#include "engine/graphics/batchManager/batchManager.hpp"
#include "engine/graphics/gpuBuffers/gpuBuffers.hpp"
#include "engine/graphics/shaderManager/shaderManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include "engine/scene/scene.hpp"
#include <GLFW/glfw3.h>
#include <glad/gl.h>

class Renderer {
private:
  BatchManager batchManager;
  MeshManager &meshManager;
  GpuBuffers gpu;
  ShaderManager shaderManager;

public:
  Renderer(MeshManager &manager);
  Renderer(const Renderer &) = delete;
  Renderer &operator=(const Renderer &) = delete;
  Renderer(Renderer &&other) = delete;
  static void initGLAD();

  void collectAndBatch(Scene &scene);
  void renderBatches();
  void flush();
};
