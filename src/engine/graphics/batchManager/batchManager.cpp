#include "./batchManager.hpp"
#include "engine/entityManager/component/components.hpp"

BatchKey BatchManager::submit(EntityId entity) {
  auto renderComp = entityManager.componentManager.getComponent<ComponentType::RENDER>(entity);
  BatchKey key = {.mesh = renderComp.mesh,
                  .material = renderComp.material};
  if (!batches.contains(key)) {
    batches[key] = Batch(key);
  }
  batches[key].submit(entity);
  batches[key].addTransform(entityManager.makeModelMatrix(entity));
  batches[key].addColor(entityManager.colorToVec4(entity));
  LOG_DEBUG("submitting entity:{} into batch with: mesh:{} material:{}",entity,key.mesh,key.material);
  return key;
}
