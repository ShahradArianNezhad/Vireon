#pragma once
#include "engine/entityManager/component/components.hpp"
#include "engine/materialManager/materialManager.hpp"
#include <cstddef>
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>

struct BatchKey {
  MeshID mesh;
  MaterialID material;
  int zIndex;
  bool operator==(const BatchKey &other) const {
    if (other.mesh == mesh && other.material == material && other.zIndex==zIndex) {
      return true;
    }
    return false;
  };
};

namespace std {
  template <>
  struct hash<BatchKey> {
    size_t operator()(const BatchKey& key) const {
      size_t h1 = hash<MeshID>()(key.mesh);
      size_t h2 = hash<MaterialID>()(key.material);
      size_t h3 = hash<int>()(key.zIndex);
      return ((h1 ^ (h2 << 1)) ^ (h3 << 2));
    }
  };
}

class Batch {
private:
  std::vector<EntityId> entities;
  std::vector<mat4> transformInstances;
  std::vector<vec4> colorInstances;
  std::vector<vec4> uvInstances;
  std::vector<vec2> lightInstances;
  BatchKey key;

public:
  Batch() {};
  Batch(BatchKey aKey) : key(aKey) {};
  BatchKey getKey() { return key; };
  std::vector<EntityId> getEntities() { return entities; };
  void submit(EntityId entity) { entities.push_back(entity); }
  void addTransform(mat4 transform){transformInstances.push_back(transform);};
  void replaceModel(EntityId id,mat4 transform);
  void addColor(vec4 color){colorInstances.push_back(color);};
  void replaceColor(EntityId id,vec4 color);
  void addUv(vec4 uv){uvInstances.push_back(uv);};
  void addLight(float radius,float intensity){lightInstances.push_back(vec2{radius,intensity});};
  void replaceUv(EntityId id,vec4 uv);
  void remove(EntityId id);
  std::vector<mat4>& getModelInstanceData(){return transformInstances;};
  std::vector<vec4>& getColorInstanceData(){return colorInstances;};
  std::vector<vec4>& getUvInstanceData(){return uvInstances;};
  std::vector<vec2>& getLightInstanceData(){return lightInstances;};
  auto begin() { return entities.begin(); }
  auto end() { return entities.end(); }
  size_t size(){return entities.size();}
};
