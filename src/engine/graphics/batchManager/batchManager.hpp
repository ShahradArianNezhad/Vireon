#pragma once
#include "./batch/batch.hpp"
#include "engine/entityManager/entityManager.hpp"

class BatchManager {
private:
  std::unordered_map<BatchKey, Batch> batches;
  EntityManager& entityManager;

public:
  BatchManager(EntityManager& eManager);
  BatchKey submit(EntityId entity);
  void remove(EntityId entity);
  std::unordered_map<BatchKey, Batch> &getBatches() { return batches; }
  std::vector<std::pair<BatchKey,Batch>> getBatches2();
};
