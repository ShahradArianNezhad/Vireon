#pragma once
#include "engine/entityManager/entity/entity.hpp"
#include "engine/materialManager/materialManager.hpp"
#include <cstddef>
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>

struct BatchKey {
  MeshID mesh;
  MaterialID material;
  bool operator==(const BatchKey &other) const {
    if (other.mesh == mesh && other.material == material) {
      return true;
    }
    return false;
  };
};

namespace std {
template <> struct hash<BatchKey> {
  size_t operator()(const BatchKey &k) const noexcept {
    return std::hash<int>{}(k.mesh);
  }
};
} // namespace std

class Batch {
private:
  std::vector<Entity *> entities;
  std::vector<glm::mat4> transformInstances;
  std::vector<glm::vec4> colorInstances;
  BatchKey key;

public:
  Batch() {};
  Batch(BatchKey aKey) : key(aKey) {};
  BatchKey getKey() { return key; };
  std::vector<Entity *> getEntities() { return entities; };
  void submit(Entity *entity) { entities.push_back(entity); }
  void addTransform(glm::mat4 transform){transformInstances.push_back(transform);};
  void addColor(glm::vec4 color){colorInstances.push_back(color);};
  std::vector<glm::mat4>& getModelInstanceData(){return transformInstances;};
  std::vector<glm::vec4>& getColorInstanceData(){return colorInstances;};
  auto begin() { return entities.begin(); }
  auto end() { return entities.end(); }
  size_t size(){return entities.size();}
};
