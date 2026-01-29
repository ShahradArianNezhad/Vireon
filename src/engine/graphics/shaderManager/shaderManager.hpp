#include "./shader/shader.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include <unordered_map>
class ShaderManager {
private:
  std::unordered_map<VertexLayout, Shader> shaders;

public:
  void useShader(VertexLayout &);
  Shader &getShaderHandle(VertexLayout &layout);
};
