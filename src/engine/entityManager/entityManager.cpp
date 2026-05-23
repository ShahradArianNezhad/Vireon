#include "entityManager.hpp"
#include "engine/entityManager/component/components.hpp"
#include "engine/eventManager/eventManager.hpp"
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <iostream>

EntityId EntityManager::newEntity() {
  auto id = idManager.get();
  if(id>=entities.size()){
    entities.push_back(id);
  }else{
    entities[id]=id;
  }
  return id;
}


EntityId EntityManager::newEntity(RenderComponent renderComp,TransformComponent transformComp){
  EntityId id = newEntity();
  componentManager.setComponent<ComponentType::RENDER>(id,renderComp);
  componentManager.setComponent<ComponentType::TRANSFORM>(id,transformComp);
  LOG_DEBUG("newEntity called RenderComp:{},TransformComp:{}",renderComp,transformComp);
  return id;
}



vec3 EntityManager::getPos(EntityId id){
  if(componentManager.hasComponent<ComponentType::TRANSFORM>(id)) return componentManager.getComponent<ComponentType::TRANSFORM>(id).position;
  LOG_WARN("trying to get pos on entity with no transform");
  return {0,0,0};
}

void EntityManager::setPos(EntityId id,vec3 pos){
  if(componentManager.hasComponent<ComponentType::TRANSFORM>(id)){
    auto trans = componentManager.getComponent<ComponentType::TRANSFORM>(id);
    trans.position=pos;
    componentManager.setComponent<ComponentType::TRANSFORM>(id,trans);
  }
  else  LOG_WARN("trying to set pos on entity with no transform");
}



mat4 EntityManager::makeModelMatrix(EntityId id){
  mat4 transform{1.0f};
  if (componentManager.hasComponent<ComponentType::TRANSFORM>(id)){
    auto transformComp = componentManager.getComponent<ComponentType::TRANSFORM>(id);
    transform = glm::translate(transform, (transformComp).position);
    transform = glm::scale(transform, (transformComp).scale);
    transform = glm::rotate(transform, (transformComp).rotation,{0.0f,0.0f,1.0f});
  }
  return transform;
}


EntityId EntityManager::makeCamera(){
  EntityId id = newEntity();
  auto cameraComp = CameraComponent2D{};
  componentManager.setComponent<ComponentType::CAMERA2D>(id,cameraComp);
  LOG_DEBUG("make camera 2d called:id:{} cameracomp:{}",id,cameraComp);
  return id;
}

void EntityManager::deleteEntity(EntityId id){
  componentManager.deleteComponent<ComponentType::TRANSFORM>(id);
  componentManager.deleteComponent<ComponentType::RENDER>(id);
  idManager.release(id);
  eventManager.emit(EntityDestroyedEvent{.id=id});
  LOG_DEBUG("entity delete called on: {}",id);
}


vec4 EntityManager::colorToVec4(EntityId id){
  auto color = componentManager.getComponent<ComponentType::RENDER>(id).color;
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
  auto comp = componentManager.getComponent<ComponentType::RENDER>(id);
  comp.color=color;
  componentManager.setComponent<ComponentType::RENDER>(id, comp);
  LOG_DEBUG("set Color 0X{} on entity:{}",color,id);
}




bool EntityManager::isColliding(EntityId e1,EntityId e2){
  if(!(componentManager.hasComponent<ComponentType::CIRCLECOLLIDER>(e1) && componentManager.hasComponent<ComponentType::RECTCOLLIDER>(e1)) ||
      (componentManager.hasComponent<ComponentType::CIRCLECOLLIDER>(e2) && componentManager.hasComponent<ComponentType::RECTCOLLIDER>(e2))){
    std::cout << "warning : collision checking on an entity thats missing a collider component"<< std::endl;
    return false;
  }
  if(componentManager.hasComponent<ComponentType::CIRCLECOLLIDER>(e1) && componentManager.hasComponent<ComponentType::CIRCLECOLLIDER>(e2)) return handleCircleCollision(e1,e2);
}



bool EntityManager::handleCircleCollision(EntityId e1,EntityId e2){
  auto t1 = componentManager.getComponent<ComponentType::TRANSFORM>(e1);
  auto t2 = componentManager.getComponent<ComponentType::TRANSFORM>(e2);
  auto c1 = componentManager.getComponent<ComponentType::CIRCLECOLLIDER>(e1);
  auto c2 = componentManager.getComponent<ComponentType::CIRCLECOLLIDER>(e2);
  auto p1 = vec2(t1.position.x,t1.position.y) + c1.offset;
  auto p2 = vec2(t2.position.x,t2.position.y) + c2.offset;
  auto d =sqrt(((p1.x-p2.x)*(p1.x-p2.x)) + ((p1.y-p2.y)*(p1.y-p2.y)));
  if(d<=c1.radius+c2.radius)return true;
  return false;
}
