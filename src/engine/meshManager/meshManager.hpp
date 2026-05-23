#pragma once
#include "./mesh/mesh.hpp"
#include "utils/idManager/idManager.hpp"
#include <cstdint>
#include "utils/hasher64/hasher64.hpp"
#include "utils/logger/logger.hpp"
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



enum class Primitive { Triangle = 0, Square,SquareSprite,Circle};

constexpr std::string_view to_string(Primitive c) {
  switch (c) {
    case Primitive::Triangle:   return "Triangle";
    case Primitive::Square: return "Square";
    case Primitive::SquareSprite:  return "SquareSprite";
    case Primitive::Circle:  return "Circle";
  }
  return "Unknown";
}

template <>
struct std::formatter<Primitive> : std::formatter<std::string_view> {
  auto format(Primitive c, format_context& ctx) const {
    return std::formatter<std::string_view>::format(
        to_string(c),
        ctx
        );
  }
};



class MeshManager {
private:
  IDManager idManager{};
  std::unordered_map<MeshID, Mesh> meshes;
  std::unordered_map<MeshSignature,MeshID> meshCache;

public:

  MeshManager();
  Mesh &get(MeshID id) {
    if(!meshes.contains(id)) LOG_FATAL("get mesh called on non existing id:{}",id);
    return meshes[id]; 
  };
  MeshID makePrimitive(Primitive);

  MeshID submit(Mesh mesh) {
    //LOG_DEBUG("mesh submitted, vertices:{},indices:{},layout:{}",mesh.vertices,mesh.indices,mesh.layout);
    auto id = idManager.get();
    meshes.emplace(id, mesh);
    return id;
  }
};
