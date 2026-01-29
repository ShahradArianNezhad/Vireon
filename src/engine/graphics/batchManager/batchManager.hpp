#pragma once
#include "./batch/batch.hpp"
#include "engine/scene/entity/entity.hpp"
#include <unordered_map>

class BatchManager {
private:
  std::unordered_map<BatchKey, Batch> batches;

public:
  BatchManager() {};
  void submit(Entity &entity);
  std::unordered_map<BatchKey, Batch> &getBatches() { return batches; }
  void cleanBatches() { batches.clear(); }
};
