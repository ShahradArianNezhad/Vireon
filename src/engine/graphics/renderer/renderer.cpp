#include "renderer.hpp"
#include "engine/entityManager/component/components.hpp"
#include "engine/eventManager/eventManager.hpp"
#include "engine/graphics/gpuBuffers/vertexBuffer/vertexBuffer.hpp"
#include "engine/sceneManager/sceneManager.hpp"
#include "glad/gl.h"
#include "platform/window/GLFWwindow.hpp"
#include <cstdint>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>



void Renderer::flush() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::initGLAD() {
  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    LOG_FATAL("Failed to initialize glad");
  }
  LOG_INFO("initializing GLAD");
}
Renderer::Renderer(MeshManager &manager, MaterialManager &matManager,SceneManager& sManager,EntityManager& eManager)
    : meshManager(manager), materialManager(matManager),sceneManager(sManager),entityManager(eManager) {
  LOG_INFO("initializing Renderer");
  EventManager::subscribe<WindowSizeChangeEvent>([this](WindowSizeChangeEvent e){windowResizeCallback();});
  enableBlending();
  setDepthParams();
  screenW=Screen::width;
  screenH=Screen::height;
  initRenderBuffer();
  initLightBuffer();
}

void Renderer::enableBlending(){
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void Renderer::setDepthParams(){
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_FALSE);
}


void Renderer::addEntity(EntityId e){
  sceneManager.get(currentScene)->addEntity(e);
}

void Renderer::initLightBuffer(){
  lightBuffer.bind();
  lightTexture.setTex(Screen::width, Screen::height, NULL);
  lightBuffer.attachTexture(lightTexture);
  lightBuffer.checkComplete();
}


void Renderer::initRenderBuffer(){
  sceneBuffer.bind();
  sceneTexture.setTex(Screen::width, Screen::height, NULL);
  sceneBuffer.attachTexture(sceneTexture);
  sceneRenderBuffer.bind();
  sceneRenderBuffer.setRenderBufferSize(Screen::width, Screen::height);
  sceneRenderBuffer.useOnFrameBuffer();
  sceneBuffer.checkComplete();
}

void Renderer::collectAndBatch(Scene *scene) {
  for (auto entityId : scene->collectEntities()) {
    if (isInScreen(entityId) && entityManager.componentManager.hasComponent<ComponentType::RENDER>(entityId))
      batchManager.submit(entityId);
  }
}

mat4 Renderer::getProjectionMatrix() {
  auto cam = sceneManager.get(currentScene)->getActiveCamera();
  auto camComp = entityManager.componentManager.getComponent<ComponentType::CAMERA2D>(cam);
  float halfW = static_cast<float>(screenW)*0.5f/camComp.zoom;
  float halfH = static_cast<float>(screenH)*0.5f/camComp.zoom;
  return glm::ortho(
    camComp.position.x - halfW,
    camComp.position.x + halfW,
    camComp.position.y + halfH,
    camComp.position.y - halfH,
    -100.0f,
    100.0f
);
}


void Renderer::renderBatches() {

#ifdef ENGINE_DEBUG
  size_t renderCalls=0;
#endif
  enableAlphaBlending();
  auto view = getViewMatrix();
  auto proj = getProjectionMatrix();
  for (auto &[key, batch] : batchManager.getBatches2()) {
    auto &mesh = meshManager.get(key.mesh);
    auto &mat = materialManager.get(key.material);
    auto &shader = shaderManager.getShaderHandle(mesh.layout);
    shader.use();
    VertexBuffer modelInstanceVBO = gpu.makeInstanceVBO(batch.getModelInstanceData());
    gpu.useInstanceMat4(modelInstanceVBO,3);
    VertexBuffer colorInstanceVBO = gpu.makeInstanceVBO(batch.getColorInstanceData());
    gpu.useInstanceVec4(colorInstanceVBO,2);
    VertexBuffer uvInstanceVBO = gpu.makeInstanceVBO(batch.getUvInstanceData());
    if(batch.getUvInstanceData().size()>0)gpu.useInstanceVec4(uvInstanceVBO,7);
    shader.setunifotmMat4("projection",proj);
    shader.setunifotmMat4("view", view);
    gpu.useMesh(mesh);
    mat.use();
    glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, batch.size());
    LOG_DEBUG("rendering batch with meshId:{} and matId:{}",key.mesh,key.material);
#ifdef ENGINE_DEBUG
    renderCalls+=1;
#endif
  }
  getGlErrors();
#ifdef ENGINE_DEBUG
  LOG_DEBUG("render calls this frame:{}",renderCalls);
#endif
}

void Renderer::enableAlphaBlending(){
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Renderer::renderSceneToBuffer(){
  sceneBuffer.bind();
  flush();
  renderBatches();
}

void Renderer::renderBufferToScreen(){
  FrameBuffer::bindScreen();
  flush();
  shaderManager.usePP();
  auto& shader=shaderManager.getPP();
  auto &mesh = meshManager.get(meshManager.makeQuad());
  lightTexture.activate(GL_TEXTURE1);
  glUniform1i(glGetUniformLocation(shader.getID(),"LightTexture"),1);
  sceneTexture.activate(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(shader.getID(),"SceneTexture"),0);
  shader.setuniformFloat("ambient", ambient);

  gpu.useMesh(mesh);
  glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, 1);
}

void Renderer::windowResizeCallback(){
    screenW=Screen::width;
    screenH=Screen::height;
    glViewport(0,0,screenW,screenH);
    sceneTexture.setTex(screenW, screenH, nullptr);
    lightTexture.setTex(screenW, screenH, nullptr);
    sceneRenderBuffer.bind();
    sceneRenderBuffer.setRenderBufferSize(screenW, screenH);
}


void Renderer::renderLights(){
  size_t count=0;
  std::vector<mat4> model;
  std::vector<vec3> color;
  std::vector<float> radius;
  std::vector<float> intensity;
  for(auto id: sceneManager.get(currentScene)->collectEntities()){
    if(entityManager.componentManager.hasComponent<ComponentType::LIGHT>(id)){
      auto lightComp = entityManager.componentManager.getComponent<ComponentType::LIGHT>(id);
      color.push_back(lightComp.color);
      radius.push_back(lightComp.radius);
      intensity.push_back(lightComp.intensity);
      model.push_back(entityManager.makeModelMatrix(id));
      count+=1;
    }
  }
  drawLights(model,color,radius,intensity,count);
};


void Renderer::drawLights(std::vector<mat4>& model,std::vector<vec3>& color,std::vector<float>& radius,std::vector<float>& intensity,size_t count){
  auto view = getViewMatrix();
  auto proj = getProjectionMatrix();
  lightBuffer.bind();
  flush();
  shaderManager.useLight();
  auto& shader = shaderManager.getLight();
  shader.setunifotmMat4("projection",proj);
  shader.setunifotmMat4("view", view);
  auto mesh = meshManager.get(meshManager.makeQuad());
  gpu.useMesh(mesh);
  lightTexture.bind();
  VertexBuffer colorInstanceVBO = gpu.makeInstanceVBO(color);
  gpu.useInstanceVec3(colorInstanceVBO,2);
  VertexBuffer radiusInstanceVBO = gpu.makeInstanceVBO(radius);
  gpu.useInstanceFloat(radiusInstanceVBO,3);
  VertexBuffer intensityInstanceVBO = gpu.makeInstanceVBO(intensity);
  gpu.useInstanceFloat(intensityInstanceVBO,4);
  VertexBuffer modelInstanceVBO = gpu.makeInstanceVBO(model);
  gpu.useInstanceMat4(modelInstanceVBO,5);
  enableAdditiveBlending();
  glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, count);
}

void Renderer::enableAdditiveBlending(){
  glBlendFunc(GL_ONE, GL_ONE);
}


void Renderer::renderCurrentScene() {
  if(sceneManager.get(currentScene)->getActiveCamera()==UINT32_MAX){
    LOG_WARN("not using a camera");
    return;
  }
  renderSceneToBuffer();
  renderLights();
  renderBufferToScreen();
  //if(screenH!=Screen::height || screenW!=Screen::width)windowResizeCallback();
}

mat4 Renderer::getViewMatrix(){
  mat4 view = mat4(1.0f);
  auto camera = sceneManager.get(currentScene)->getActiveCamera();
  auto camComp = entityManager.componentManager.getComponent<ComponentType::CAMERA2D>(camera);
  view = glm::rotate(
      view,
      glm::radians(-camComp.rotation),
      glm::vec3(0.0f, 0.0f, 1.0f)
      );
  view = glm::translate(view, vec3(-camComp.position.x,-camComp.position.y,0.0f));
  return view;
}

void Renderer::getGlErrors(){
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    LOG_ERROR("openGL error: {}",err);
  }
}


bool Renderer::isInScreen(EntityId id){
  auto camera = sceneManager.get(currentScene)->getActiveCamera();
  auto camPos = entityManager.componentManager.getComponent<ComponentType::CAMERA2D>(camera);
  auto camRight =camPos.position.x + ((screenW)/2.0);
  auto camLeft = camPos.position.x - ((screenW)/2.0) ;
  auto camTop = camPos.position.y - ((screenH)/2.0) ;
  auto camBot = camPos.position.y + ((screenH)/2.0);
  auto entityPos = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(id);
  auto entityRight = entityPos.position.x + (entityPos.scale.x/1.0);
  auto entityLeft = entityPos.position.x - (entityPos.scale.x/1.0);
  auto entityTop = entityPos.position.y - (entityPos.scale.y/1.0);
  auto entityBot = entityPos.position.y + (entityPos.scale.y/1.0);
  return !(camRight < entityLeft || camLeft > entityRight ||
      camBot < entityTop || camTop > entityBot);
}
