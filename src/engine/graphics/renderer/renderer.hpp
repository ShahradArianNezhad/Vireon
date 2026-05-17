#pragma once
#include "engine/entityManager/entity/entity.hpp"
#include "engine/materialManager/materialManager.hpp"
#define GLFW_INCLUDE_NONE
#include "engine/graphics/batchManager/batchManager.hpp"
#include "engine/graphics/gpuBuffers/gpuBuffers.hpp"
#include "engine/graphics/shaderManager/shaderManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include "engine/sceneManager/scene/scene.hpp"
#include "engine/sceneManager/sceneManager.hpp"
#include <GLFW/glfw3.h>
#include <glad/gl.h>

class Renderer {
private:
  BatchManager batchManager;
  MeshManager &meshManager;
  MaterialManager &materialManager;
  SceneManager& sceneManager;
  GpuBuffers gpu;
  ShaderManager shaderManager;
  Scene *currentScene=nullptr;
  static glm::mat4 projectionMatrix;

  void getGlErrors();
  void collectAndBatch(Scene *scene);
  void renderBatches();
  glm::mat4 getViewMatrix();
  static glm::mat4 getProjectionMatrix();
  VertexBuffer makeColorInstanceVBO(Batch& batch);
  VertexBuffer makeModelInstanceVBO(Batch& batch);

public:
  Renderer(MeshManager &manager, MaterialManager &matManager,SceneManager& sceneManager);
  Renderer(const Renderer &) = delete;
  Renderer &operator=(const Renderer &) = delete;
  Renderer(Renderer &&other) = delete;
  static void initGLAD();
  static void recalculateProjectionMatrix(){Renderer::projectionMatrix=getProjectionMatrix();};

  void useScene(SceneId scene) { currentScene = sceneManager.get(scene); };
  void addEntity(Entity* e){currentScene->addEntity(e);}
  void flush();
  void renderCurrentScene();

};
