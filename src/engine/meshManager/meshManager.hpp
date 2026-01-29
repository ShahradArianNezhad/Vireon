#pragma once
#include "./mesh/mesh.hpp"
#include "core/idManager/idManager.hpp"
#include <unordered_map>

using MeshID = unsigned int;

class MeshManager {
private:
  IDManager idManager;
  std::unordered_map<MeshID, Mesh> meshes;

public:
  MeshManager() {};
  Mesh &get(MeshID id) { return meshes[id]; };
  MeshID submit(Mesh mesh) {
    auto id = idManager.get();
    meshes.emplace(id, mesh);
    return id;
  }
};
