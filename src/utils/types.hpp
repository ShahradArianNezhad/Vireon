#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <string>

class AllocatorStats{
  public:
    size_t allocatedBytes;
    std::string allocatorName;
};

enum Layer{
  WORLD=0,
  UI
};

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat3 = glm::mat3;
using mat4 = glm::mat4;
