#pragma once
#include "./material/material.hpp"
#include "core/idManager/idManager.hpp"
#include <unordered_map>

using MaterialID = unsigned int;

class MaterialManager {
private:
  IDManager idManager;
  std::unordered_map<MaterialID, Material> materials;

public:
  MaterialManager() {};
  Material &get(MaterialID id) { return materials[id]; };
  MaterialID newMat() {
    auto id = idManager.get();
    materials.emplace(id,Material{});
    return id;
  }
};
