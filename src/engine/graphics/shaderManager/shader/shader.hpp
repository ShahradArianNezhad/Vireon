#pragma once
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include "glad/gl.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <string>

class Shader {
private:
  unsigned int vShader, fShader;
  unsigned int programID;
  void checkCompileErrors(unsigned int shaderID);
  void checkProgramErrors();
  void compileShader(const std::string &path, unsigned int dest);
  void makeAndLinkProgram();
  void cleanShaders();
  void makeAndCompileShaders(const std::string &vertexShaderPath,
                             const std::string &fragmentShaderPath);

public:
  Shader() {};
  Shader(VertexLayout layout);
  Shader(const std::string &);
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;
  Shader(Shader &&other) noexcept;

  void use() const { glUseProgram(programID); }
  unsigned int getID() { return programID; }
};
