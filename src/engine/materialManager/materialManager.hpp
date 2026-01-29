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
  MaterialID submit(Material material) {
    auto id = idManager.get();
    materials.insert({id, std::move(material)});
    return id;
  }
};
