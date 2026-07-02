#include "./batchManager.hpp"
#include "engine/entityManager/ComponentManager/componentManager.hpp"
#include "engine/entityManager/component/components.hpp"
#include "engine/entityManager/entityManager.hpp"
#include "engine/eventManager/eventManager.hpp"
#include <algorithm>



BatchManager::BatchManager(Layer batchLayer,EntityManager& eManager):layer(batchLayer),entityManager(eManager){
  EventManager::subscribe<EntityCreatedEvent>([this](EntityCreatedEvent e){
      if(e.layer==layer)submit(e.id);
  });
  EventManager::subscribe<EntityDestroyedEvent>([this](EntityDestroyedEvent e){remove(e.id);});
  EventManager::subscribe<ComponentChangingEvent<Component::TRANSFORM>>([this](ComponentChangingEvent<Component::TRANSFORM> e){
      if(!entityManager.componentManager.hasComponent<Component::RENDER,Component::TRANSFORM>(e.entity))return;
      auto renderComp = entityManager.componentManager.getComponent<Component::RENDER>(e.entity);
      auto transformComp = entityManager.componentManager.getComponent<Component::TRANSFORM>(e.entity);
      BatchKey key = {.mesh = renderComp.mesh,
      .material = renderComp.material,
      .zIndex=(int)transformComp.position.z};
      if (!batches.contains(key))return;
      if(e.newComp.position.z != transformComp.position.z && renderComp.visible){
        batches[key].remove(e.entity);
        submit(e.entity,e.newComp,renderComp);
      }else batches[key].replaceModel(e.entity, entityManager.makeModelMatrix(e.newComp));
  });
  EventManager::subscribe<ComponentChangingEvent<Component::RENDER>>([this](ComponentChangingEvent<Component::RENDER> e){
      if(!entityManager.componentManager.hasComponent<Component::RENDER,Component::TRANSFORM>(e.entity))return;
      auto renderComp = entityManager.componentManager.getComponent<Component::RENDER>(e.entity);
      auto transformComp = entityManager.componentManager.getComponent<Component::TRANSFORM>(e.entity);
      BatchKey key = {.mesh = renderComp.mesh,
      .material = renderComp.material,
      .zIndex=(int)transformComp.position.z};
      if (!batches.contains(key))return;
      if(renderComp.visible!=e.newComp.visible){
        if(e.newComp.visible)submit(e.entity,transformComp,e.newComp);
        else batches[key].remove(e.entity);
      }else if(renderComp.mesh != e.newComp.mesh || renderComp.material != e.newComp.material){
        batches[key].remove(e.entity);
        submit(e.entity,transformComp,e.newComp);
      }
      else batches[key].replaceColor(e.entity, entityManager.colorToVec4(e.newComp.color));
  });
  EventManager::subscribe<ComponentChangingEvent<Component::UVRECT>>([this](ComponentChangingEvent<Component::UVRECT> e){
      auto renderComp = entityManager.componentManager.getComponent<Component::RENDER>(e.entity);
      auto transformComp = entityManager.componentManager.getComponent<Component::TRANSFORM>(e.entity);
      BatchKey key = {.mesh = renderComp.mesh,
      .material = renderComp.material,
      .zIndex=(int)transformComp.position.z};
      if (!batches.contains(key))return;
      batches[key].replaceUv(e.entity, {e.newComp.uvMin,e.newComp.uvMax});
  });
};

BatchKey BatchManager::submit(EntityId entity,Component::TRANSFORM& transformComp,Component::RENDER& renderComp){
  BatchKey key = {.mesh = renderComp.mesh,
    .material = renderComp.material,
    .zIndex=(int)transformComp.position.z};
  if(!batches.contains(key))batches.emplace(key,key);
  batches[key].submit(entity);
  batches[key].addTransform(entityManager.makeModelMatrix(transformComp));
  batches[key].addColor(entityManager.getColorVec4(entity));
  if(entityManager.componentManager.hasComponent<Component::UVRECT>(entity)){
    auto uv = entityManager.componentManager.getComponent<Component::UVRECT>(entity);
    batches[key].addUv(vec4{uv.uvMin,uv.uvMax});
  }
#ifdef DEBUG_VERBOSE
  LOG_DEBUG("submitting entity:{} into batch with: mesh:{} material:{},zIndex:{}",entity,key.mesh,key.material,(int)transformComp.position.z);
#endif
  return key;
}

BatchKey BatchManager::submit(EntityId entity){
  if(!entityManager.componentManager.hasComponent<Component::RENDER>(entity))return BatchKey{};
  auto renderComp = entityManager.componentManager.getComponent<Component::RENDER>(entity);
  auto transformComp = entityManager.componentManager.getComponent<Component::TRANSFORM>(entity);
  return submit(entity,transformComp,renderComp);
}


void BatchManager::remove(EntityId entity){
  if(!entityManager.componentManager.hasComponent<Component::RENDER>(entity))return;
  auto renderComp = entityManager.componentManager.getComponent<Component::RENDER>(entity);
  auto transformComp = entityManager.componentManager.getComponent<Component::TRANSFORM>(entity);
  BatchKey key = {.mesh = renderComp.mesh,
                  .material = renderComp.material,
                  .zIndex=(int)transformComp.position.z};
  if (!batches.contains(key)){
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
