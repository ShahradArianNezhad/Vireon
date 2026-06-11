#include "./batchManager.hpp"
#include "engine/entityManager/ComponentManager/componentManager.hpp"
#include "engine/entityManager/component/components.hpp"
#include "engine/entityManager/entityManager.hpp"
#include "engine/eventManager/eventManager.hpp"
#include <algorithm>



BatchManager::BatchManager(EntityManager& eManager):entityManager(eManager) {
  EventManager::subscribe<EntityCreatedEvent>([this](EntityCreatedEvent e){submit(e.id);});
  EventManager::subscribe<EntityDestroyedEvent>([this](EntityDestroyedEvent e){remove(e.id);});
  EventManager::subscribe<ComponentSetEvent<TransformComponent>>([this](ComponentSetEvent<TransformComponent> e){
      if(!entityManager.componentManager.hasComponent<ComponentType::RENDER,ComponentType::TRANSFORM>(e.entity))return;
      auto renderComp = entityManager.componentManager.getComponent<ComponentType::RENDER>(e.entity);
      auto transformComp = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(e.entity);
      BatchKey key = {.mesh = renderComp.mesh,
      .material = renderComp.material,
      .zIndex=(int)transformComp.position.z};
      if (!batches.contains(key))return;
      batches[key].replaceModel(e.entity, entityManager.makeModelMatrix(e.entity));
  });
  EventManager::subscribe<ComponentSetEvent<RenderComponent>>([this](ComponentSetEvent<RenderComponent> e){
      if(!entityManager.componentManager.hasComponent<ComponentType::RENDER,ComponentType::TRANSFORM>(e.entity))return;
      auto renderComp = entityManager.componentManager.getComponent<ComponentType::RENDER>(e.entity);
      auto transformComp = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(e.entity);
      BatchKey key = {.mesh = renderComp.mesh,
      .material = renderComp.material,
      .zIndex=(int)transformComp.position.z};
      if (!batches.contains(key))return;
      batches[key].replaceColor(e.entity, entityManager.colorToVec4(e.entity));
  });
  EventManager::subscribe<ComponentSetEvent<UvRectComponent>>([this](ComponentSetEvent<UvRectComponent> e){
      auto renderComp = entityManager.componentManager.getComponent<ComponentType::RENDER>(e.entity);
      auto transformComp = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(e.entity);
      BatchKey key = {.mesh = renderComp.mesh,
      .material = renderComp.material,
      .zIndex=(int)transformComp.position.z};
      if (!batches.contains(key))return;
      batches[key].replaceUv(e.entity, {e.comp.uvMin,e.comp.uvMax});
  });
};


BatchKey BatchManager::submit(EntityId entity) {
  auto renderComp = entityManager.componentManager.getComponent<ComponentType::RENDER>(entity);
  auto transformComp = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(entity);
  BatchKey key = {.mesh = renderComp.mesh,
                  .material = renderComp.material,
                  .zIndex=(int)transformComp.position.z};
  if (!batches.contains(key)) {
    batches[key] = Batch(key);
  }
  batches[key].submit(entity);
  batches[key].addTransform(entityManager.makeModelMatrix(entity));
  batches[key].addColor(entityManager.colorToVec4(entity));
  if(entityManager.componentManager.hasComponent<ComponentType::UVRECT>(entity)){
    auto uv = entityManager.componentManager.getComponent<ComponentType::UVRECT>(entity);
    batches[key].addUv(vec4{uv.uvMin,uv.uvMax});
  }
  LOG_DEBUG("submitting entity:{} into batch with: mesh:{} material:{},zIndex:{}",entity,key.mesh,key.material,(int)transformComp.position.z);
  return key;
}


void BatchManager::remove(EntityId entity){
  auto renderComp = entityManager.componentManager.getComponent<ComponentType::RENDER>(entity);
  auto transformComp = entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(entity);
  BatchKey key = {.mesh = renderComp.mesh,
                  .material = renderComp.material,
                  .zIndex=(int)transformComp.position.z};
  if (!batches.contains(key)){
    LOG_WARN("trying to remove an entity from batch that doesnt exist in batches entityID : {}",entity);
    return;
  }
  batches[key].remove(entity);
  if(batches[key].size()==0)batches.erase(key);
}




std::vector<std::pair<BatchKey,Batch>> BatchManager::getBatches2(){
  std::vector<std::pair<BatchKey,Batch>> sorted;
  sorted.reserve(batches.size());
  for(auto& [key,value]: batches)sorted.emplace_back(key,value);
  std::sort(sorted.begin(),sorted.end(),[](const auto& a,const auto& b){
      return a.first.zIndex < b.first.zIndex;
      });
  return sorted;
}
