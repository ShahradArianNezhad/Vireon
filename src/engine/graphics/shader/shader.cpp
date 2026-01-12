#include "engine/graphics/shader/shader.hpp"
#include "core/file/fileIO.hpp"
#include "glad/gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(const std::string &vertexShaderPath,
               const std::string &fragmentShaderPath) {
  unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
  const std::string vShaderStr = FileIO::readText(vertexShaderPath);
  const char *vShaderC_str = vShaderStr.c_str();
  glShaderSource(vShader, 1, &vShaderC_str, nullptr);
  glCompileShader(vShader);
  checkCompileErrors(vShader);

  unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
  const std::string fShaderStr = FileIO::readText(fragmentShaderPath);
  const char *fShaderC_str = fShaderStr.c_str();
  glShaderSource(fShader, 1, &fShaderC_str, nullptr);
  glCompileShader(fShader);
  checkCompileErrors(fShader);

  programID = glCreateProgram();
  glAttachShader(programID, vShader);
  glAttachShader(programID, fShader);
  glLinkProgram(programID);
  checkProgramErrors();

  glDeleteShader(vShader);
  glDeleteShader(fShader);
}

void Shader::checkCompileErrors(unsigned int shaderID) {
  int success;
  char infoLog[512];
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
    std::cout << "ERROR: SHADER COMPILATION FAILED= " << infoLog << std::endl;
  }
}

void Shader::checkProgramErrors() {
  int success;
  char infoLog[512];
  glGetProgramiv(programID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(programID, 512, NULL, infoLog);
    std::cout << "ERROR: PROGRAM LINKING FAILED= " << infoLog << std::endl;
  }
}

Shader::Shader(Shader &&other) noexcept {
  programID = other.programID;
  other.programID = 0;
}

void Shader::setUniformVec4(const std::string &name, float r, float g, float b,
                            float a) {
  int uniformLocation = glGetUniformLocation(programID, name.c_str());
  if (uniformLocation == -1) {
    std::cout << "ERROR: uniform location '" << name << "' was not found"
              << std::endl;
  }
  use();
  glUniform4f(uniformLocation, r, g, b, a);
}

void Shader::setUniformMat4(const std::string &name, glm::mat4 matrix) {
  int uniformLocation = glGetUniformLocation(programID, name.c_str());
  if (uniformLocation == -1) {
    std::cout << "ERROR: uniform location '" << name << "' was not found"
              << std::endl;
  }
  use();
  glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(matrix));
}
