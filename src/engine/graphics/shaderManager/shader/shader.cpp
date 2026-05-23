#include "./shader.hpp"
#include "utils/file/fileIO.hpp"
#include "glad/gl.h"
#include "utils/logger/logger.hpp"
#include "utils/types.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(VertexLayout layout) {
  if (layout == VertexLayout::Pos) {
    makeAndCompileShaders("./shaders/simple");
    makeAndLinkProgram();
    cleanShaders();
  } else if (layout == VertexLayout::PosUV){
    makeAndCompileShaders("./shaders/textured");
    makeAndLinkProgram();
    cleanShaders();
  } else {
    LOG_ERROR("no shader found for specified vertexlayout");
  }
}

Shader::Shader(const std::string &shaderFolderPath) {
  makeAndCompileShaders(shaderFolderPath);
  makeAndLinkProgram();
  cleanShaders();
}

void Shader::makeAndCompileShaders(const std::string &folderPath) {
  std::string vertexShaderPath = folderPath + "/" + "shader.vert";
  std::string fragmentShaderPath = folderPath + "/" + "shader.frag";
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
    LOG_ERROR("shader compilation failed: {}",infoLog);
  }
}

void Shader::checkProgramErrors() {
  int success;
  char infoLog[512];
  glGetProgramiv(programID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(programID, 512, NULL, infoLog);
    LOG_ERROR("Shader program linking failed:{}",infoLog);
  }
}

Shader::Shader(Shader &&other) noexcept {
  programID = other.programID;
  other.programID = 0;
}

void Shader::setunifotmMat4(std::string name, mat4 &mat4) {
  GLint modelLoc = glGetUniformLocation(programID, name.c_str());
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::setunifotmVec4(std::string name, vec4 vec4) {
  GLint modelLoc = glGetUniformLocation(programID, name.c_str());
  glUniform4fv(modelLoc, 1, glm::value_ptr(vec4));
}
