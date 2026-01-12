#pragma once
#include "glad/gl.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <string>

class Shader {
private:
  unsigned int programID;
  void checkCompileErrors(unsigned int shaderID);
  void checkProgramErrors();

public:
  Shader(const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath);
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;
  Shader(Shader &&other) noexcept;

  void use() const { glUseProgram(programID); }
  void setUniformVec4(const std::string &name, float r, float g, float b,
                      float a);
  void setUniformMat4(const std::string &name, glm::mat4 matrix);
  unsigned int getID() { return programID; }
};
