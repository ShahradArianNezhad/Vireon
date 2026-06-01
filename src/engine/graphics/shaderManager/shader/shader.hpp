#pragma once
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include "glad/gl.h"
#include <cstdint>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <string>

class Shader {
private:
  unsigned int vShader, fShader;
  unsigned int programID=UINT32_MAX;
  void checkCompileErrors(unsigned int shaderID);
  void checkProgramErrors();
  void compileShader(const std::string &path, unsigned int dest);
  void makeAndLinkProgram();
  void cleanShaders();
  void makeAndCompileShaders(const std::string &folderPath);

public:
  Shader() {};
  Shader(VertexLayout layout);
  Shader(const std::string &);
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;
  Shader(Shader &&other) noexcept;

  void setunifotmMat4(std::string name, glm::mat4 &mat4);
  void setunifotmVec4(std::string name, glm::vec4 vec4);
  void setuniformFloat(std::string name,float f);
  void use() const { glUseProgram(programID); }
  unsigned int getID() { return programID; }
};
