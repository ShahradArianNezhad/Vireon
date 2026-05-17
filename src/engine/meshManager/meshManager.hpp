#pragma once
#include "./mesh/mesh.hpp"
#include "core/idManager/idManager.hpp"
#include <cstdint>
#include "core/hasher64/hasher64.hpp"
#include <unordered_map>

using MeshID = unsigned int;
using MeshSignature = uint64_t;

enum Color {
  White = 0xFFFFFFFF,
  Black = 0x000000FF,
  Red = 0xFF0000FF,
  Blue = 0x0000FFFF,
  Green = 0x00FF00FF
};

class MeshManager {
private:
  IDManager idManager{};
  std::unordered_map<MeshID, Mesh> meshes;
  std::unordered_map<MeshSignature,MeshID> meshCache;

public:
  enum class Primitive { Triangle = 0, Square,SquareSprite,Circle };

  MeshManager();
  Mesh &get(MeshID id) { return meshes[id]; };
  MeshID makePrimitive(Primitive);
  MeshID submit(Mesh mesh) {
    auto id = idManager.get();
    meshes.emplace(id, mesh);
    return id;
  }
};
