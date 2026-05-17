#include "./batchManager.hpp"

BatchKey BatchManager::submit(Entity *entity) {
  BatchKey key = {.mesh = entity->renderComp->mesh,
                  .material = entity->renderComp->material};
  if (!batches.contains(key)) {
    batches[key] = Batch(key);
  }
  batches[key].submit(entity);
  batches[key].addTransform(entity->getModelMatrix());
  batches[key].addColor(entity->tintToVec4());
  return key;
}
