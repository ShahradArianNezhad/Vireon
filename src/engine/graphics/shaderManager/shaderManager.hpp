#pragma once
#include "./shader/shader.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include <unordered_map>
class ShaderManager {
private:
  std::unordered_map<VertexLayout, Shader> shaders;
  std::optional<Shader> ppShader;
  std::optional<Shader> lightShader;

public:
  void useLight(){
    if(!lightShader) lightShader.emplace("shaders/light");
    lightShader->use();
  }
  Shader& getLight(){
    if(!lightShader) lightShader.emplace("shaders/light");
    return *lightShader;
  }
  void useShader(VertexLayout &);
  void usePP(){
    if(!ppShader) ppShader.emplace("shaders/secondPass");
    ppShader->use();
  }
  Shader& getPP(){
    if(!ppShader) ppShader.emplace("shaders/secondPass");
    return *ppShader;
  }
  Shader &getShaderHandle(VertexLayout &layout);
};
