#pragma once
#include "./mesh/mesh.hpp"
#include "core/idManager/idManager.hpp"
#include <cstdint>
#include <functional>
#include <unordered_map>

using MeshID = unsigned int;
using MeshSignature = uint64_t;

enum class Color {
  White = 0xFFFFFF,
  Black = 0x000000,
  Red = 0xFF0000,
  Blue = 0x0000FF,
  Green = 0x00FF00
};

class MeshManager {
private:
  IDManager idManager{};
  std::unordered_map<MeshID, Mesh> meshes;
  std::unordered_map<MeshSignature,MeshID> meshCache;

public:
  enum class Primitive { Triangle = 0, Square };

  MeshManager();
  Mesh &get(MeshID id) { return meshes[id]; };
  MeshID makePrimitive(Primitive, Color);
  MeshID submit(Mesh mesh) {
    auto id = idManager.get();
    meshes.emplace(id, mesh);
    return id;
  }
};
