#include "./batchManager.hpp"
#include "engine/entityManager/component/components.hpp"
#include <algorithm>

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




std::vector<std::pair<BatchKey,Batch>> BatchManager::getBatches2(){
  std::vector<std::pair<BatchKey,Batch>> sorted;
  sorted.reserve(batches.size());
  for(auto& [key,value]: batches)sorted.emplace_back(key,value);
  std::sort(sorted.begin(),sorted.end(),[](const auto& a,const auto& b){
      return a.first.zIndex < b.first.zIndex;
      });
  return sorted;
}
