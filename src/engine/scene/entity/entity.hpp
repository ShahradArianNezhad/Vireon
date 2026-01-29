#pragma once
#include "engine/materialManager/materialManager.hpp"
#include "engine/meshManager/meshManager.hpp"

struct Entity {
  MeshID mesh;
  MaterialID material;
  bool visible;
};
