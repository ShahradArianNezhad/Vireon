#pragma once
#include "./batch/batch.hpp"
#include "engine/entityManager/entityManager.hpp"

class BatchManager {
private:
  std::unordered_map<BatchKey, Batch> batches;
  Layer layer;
  EntityManager& entityManager;

public:
  BatchManager(Layer batchLayer,EntityManager& eManager);
  BatchKey submit(EntityId entity);
  BatchKey submit(EntityId entity,Component::TRANSFORM& transformComp,Component::RENDER& renderComp);
  void remove(EntityId entity);
  std::unordered_map<BatchKey, Batch> &getBatches() { return batches; }
  std::vector<std::pair<BatchKey,Batch>> getBatches2();
};
