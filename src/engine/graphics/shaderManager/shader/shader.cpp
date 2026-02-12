#include "./shader.hpp"
#include "core/file/fileIO.hpp"
#include "glad/gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(VertexLayout layout) {
  if (layout == VertexLayout::Pos) {
    makeAndCompileShaders(std::string("./shaders/simple") + "/" + "shader.vert",
                          std::string("./shaders/simple") + "/" +
                              "shader.frag");
    makeAndLinkProgram();
    cleanShaders();
  } else {
    std::cout << "no shader found" << std::endl;
  }
}

Shader::Shader(const std::string &shaderFolderPath) {
  makeAndCompileShaders(shaderFolderPath + "/" + "shader.vert",
                        shaderFolderPath + "/" + "shader.frag");
  makeAndLinkProgram();
  cleanShaders();
}

void Shader::makeAndCompileShaders(const std::string &vertexShaderPath,
                                   const std::string &fragmentShaderPath) {
  vShader = glCreateShader(GL_VERTEX_SHADER);
  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  compileShader(vertexShaderPath, vShader);
  compileShader(fragmentShaderPath, fShader);
}

void Shader::makeAndLinkProgram() {
  programID = glCreateProgram();
  glAttachShader(programID, vShader);
  glAttachShader(programID, fShader);
  glLinkProgram(programID);
  checkProgramErrors();
}

void Shader::cleanShaders() {
  glDeleteShader(vShader);
  glDeleteShader(fShader);
}

void Shader::compileShader(const std::string &path, unsigned int dest) {
  const std::string vShaderStr = FileIO::readText(path);
  const char *vShaderC_str = vShaderStr.c_str();
  glShaderSource(dest, 1, &vShaderC_str, nullptr);
  glCompileShader(dest);
  checkCompileErrors(dest);
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

void Shader::setunifotmMat4(std::string name, glm::mat4 &mat4) {
  GLint modelLoc = glGetUniformLocation(programID, name.c_str());
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::setunifotmVec4(std::string name, glm::vec4 vec4) {
  GLint modelLoc = glGetUniformLocation(programID, name.c_str());
  glUniform4fv(modelLoc, 1, glm::value_ptr(vec4));
}
