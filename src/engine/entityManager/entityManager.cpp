#include "entityManager.hpp"
#include "engine/entityManager/component/components.hpp"
#include "engine/eventManager/eventManager.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>

EntityId EntityManager::newEntity() {
  auto id = idManager.get();
  if(id>=entities.size()){
    entities.push_back(id);
  }else{
    entities[id]=id;
  }
  return id;
}


EntityId EntityManager::newEntity(Component::RENDER renderComp,Component::TRANSFORM transformComp){
  EntityId id = newEntity();
  componentManager.setComponent<Component::RENDER>(id,renderComp);
  componentManager.setComponent<Component::TRANSFORM>(id,transformComp);
  LOG_DEBUG("newEntity called RenderComp:{},TransformComp:{}",renderComp,transformComp);
  return id;
}



vec3 EntityManager::getPos(EntityId id){
  if(componentManager.hasComponent<Component::TRANSFORM>(id)) return componentManager.getComponent<Component::TRANSFORM>(id).position;
  LOG_WARN("trying to get pos on entity with no transform");
  return {0,0,0};
}

void EntityManager::setPos(EntityId id,vec3 pos){
  if(componentManager.hasComponent<Component::TRANSFORM>(id)){
    auto trans = componentManager.getComponent<Component::TRANSFORM>(id);
    trans.position=pos;
    componentManager.setComponent<Component::TRANSFORM>(id,trans);
  }
  else  LOG_WARN("trying to set pos on entity with no transform");
}



mat4 EntityManager::makeModelMatrix(EntityId id){
  mat4 transform{1.0f};
  if (componentManager.hasComponent<Component::TRANSFORM>(id)){
    auto transformComp = componentManager.getComponent<Component::TRANSFORM>(id);
    transform = glm::translate(transform, (transformComp).position);
    transform = glm::scale(transform, (transformComp).scale);
    transform = glm::rotate(transform, (transformComp).rotation,{0.0f,0.0f,1.0f});
  }
  return transform;
}


EntityId EntityManager::makeCamera(){
  EntityId id = newEntity();
  auto cameraComp = Component::CAMERA2D{};
  componentManager.setComponent<Component::CAMERA2D>(id,cameraComp);
  LOG_DEBUG("make camera 2d called:id:{} cameracomp:{}",id,cameraComp);
  return id;
}

void EntityManager::deleteEntity(EntityId id){
  EventManager::emit(EntityDestroyedEvent{.id=id});
  componentManager.deleteComponent<Component::TRANSFORM>(id);
  componentManager.deleteComponent<Component::RENDER>(id);
  idManager.release(id);
  LOG_DEBUG("entity delete called on: {}",id);
}


vec4 EntityManager::colorToVec4(EntityId id){
  auto color = componentManager.getComponent<Component::RENDER>(id).color;
  float r =((static_cast<unsigned int>(color)&0xFF000000)>>24)/255.0f; 
  float g =((static_cast<unsigned int>(color)&0x00FF0000)>>16)/255.0f; 
  float b =((static_cast<unsigned int>(color)&0x0000FF00)>>8)/255.0f; 
  float a =((static_cast<unsigned int>(color)&0x000000FF))/255.0f; 
  return {r,g,b,a};
}

void EntityManager::setColor(EntityId id,unsigned int r,unsigned int g,unsigned int b,unsigned int a){
  uint32_t color=0;
  color|=((r&0x000000FF)<<24);
  color|=((g&0x000000FF)<<16);
  color|=((b&0x000000FF)<<8);
  color|=(a&0x000000FF);
  auto comp = componentManager.getComponent<Component::RENDER>(id);
  comp.color=color;
  componentManager.setComponent<Component::RENDER>(id, comp);
  LOG_DEBUG("set Color 0X{} on entity:{}",color,id);
}

