#include "./batchManager.hpp"

void BatchManager::submit(Entity *entity) {
  BatchKey key = {.mesh = entity->renderComp->mesh,
                  .material = entity->renderComp->material};
  if (!batches.contains(key)) {
    batches[key] = Batch(key);
  }
  batches[key].submit(entity);
}
