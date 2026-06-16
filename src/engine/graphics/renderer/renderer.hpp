#pragma once
#include "engine/entityManager/entityManager.hpp"
#include "engine/materialManager/materialManager.hpp"
#include "platform/window/GLFWwindow.hpp"
#define GLFW_INCLUDE_NONE
#include "engine/graphics/batchManager/batchManager.hpp"
#include "engine/graphics/gpuBuffers/gpuBuffers.hpp"
#include "engine/graphics/shaderManager/shaderManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include "engine/sceneManager/scene/scene.hpp"
#include "engine/sceneManager/sceneManager.hpp"
#include "engine/graphics/gpuBuffers/frameBuffer/frameBuffer.hpp"
#include "engine/graphics/gpuBuffers/renderBuffer/renderBuffer.hpp"
#include <GLFW/glfw3.h>
#include <glad/gl.h>



class Renderer {
  private:
  FrameBuffer lightBuffer;
  Texture lightTexture;
  FrameBuffer sceneBuffer;
  Texture sceneTexture;
  RenderBuffer sceneRenderBuffer;
  MeshManager &meshManager;
  MaterialManager &materialManager;
  SceneManager& sceneManager;
  EntityManager& entityManager;
  BatchManager worldBatchManager{Layer::WORLD,entityManager};
  BatchManager UIBatchManager{Layer::UI,entityManager};
  GpuBuffers gpu;
  ShaderManager shaderManager;
  SceneId currentScene=0;

  size_t screenW,screenH;
  void getGlErrors();
  void collectAndBatch(Scene *scene);
  void renderBatches(std::vector<std::pair<BatchKey,Batch>> batches);
  mat4 getViewMatrix();
  mat4 makeModelMatrix(EntityId id);
  mat4 getProjectionMatrix();
  bool isInScreen(EntityId id);
  void renderSceneToBuffer();
  void renderBufferToScreen();
  void windowResizeCallback(WindowSizeChangeEvent e);
  void initRenderBuffer();
  void initLightBuffer();
  void drawLights(std::vector<mat4>& model,std::vector<vec3>& color,std::vector<float>& radius,std::vector<float>& intensity,size_t count);
  void renderLights();
  void enableBlending();
  void setDepthParams();
  void enableAdditiveBlending();
  void enableAlphaBlending();
  void renderUI();


public:
  Renderer(MeshManager &manager, MaterialManager &matManager,SceneManager& sceneManager,EntityManager& entityManager);
  Renderer(const Renderer &) = delete;
  Renderer &operator=(const Renderer &) = delete;
  Renderer(Renderer &&other) = delete;
  ~Renderer(){};
  static void initGLAD();

  void useScene(SceneId scene) { currentScene = scene; };
  void addEntity(EntityId e,Layer layer);
  SceneId getCurrentScene(){return currentScene;}
  void flush();
  float ambient=0.9;
  void renderCurrentScene();

};

