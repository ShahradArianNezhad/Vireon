#include "./shaderManager.hpp"

void ShaderManager::useShader(VertexLayout &layout) {
  if (!shaders.contains(layout)) {
    shaders.emplace(layout, Shader(layout));
  }
  shaders[layout].use();
}

Shader &ShaderManager::getShaderHandle(VertexLayout &layout) {
  if (!shaders.contains(layout)) {
    shaders.emplace(layout, Shader(layout));
  }
  return shaders[layout];
}
