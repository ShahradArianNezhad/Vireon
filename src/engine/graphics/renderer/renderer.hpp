#pragma once
#include "engine/materialManager/materialManager.hpp"
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
  MaterialManager &materialManager;
  GpuBuffers gpu;
  ShaderManager shaderManager;
  void collectAndBatch(Scene &scene);
  void renderBatches();

public:
  Renderer(MeshManager &manager, MaterialManager &matManager);
  Renderer(const Renderer &) = delete;
  Renderer &operator=(const Renderer &) = delete;
  Renderer(Renderer &&other) = delete;
  static void initGLAD();

  void flush();
  void renderScene(Scene &scene);
};
