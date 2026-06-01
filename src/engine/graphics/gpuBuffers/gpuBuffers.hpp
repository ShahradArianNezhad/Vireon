#pragma once
#include "./elementBuffer/elementBuffer.hpp"
#include "./vertexArray/vertexArray.hpp"
#include "./vertexBuffer/vertexBuffer.hpp"
#include "engine/meshManager/meshManager.hpp"
class GpuBuffers {
private:
  VertexArray vao;
  VertexBuffer vbo;
  ElementBuffer ebo;

public:
  void useMesh(Mesh &mesh);
  void useInstanceMat4(VertexBuffer& instanceVBO, unsigned int location);
  void useInstanceVec4(VertexBuffer& instanceVBO, unsigned int location);
  void useInstanceVec3(VertexBuffer& instanceVBO, unsigned int location);
  void useInstanceFloat(VertexBuffer& instanceVBO, unsigned int location);
  void reset();

  template <typename T>
    static VertexBuffer makeInstanceVBO(std::vector<T>& data){
      VertexBuffer instanceVBO;
      instanceVBO.upload(data,GL_STATIC_DRAW);
      //LOG_DEBUG("made instance VBO with data:{}",data);
      return instanceVBO;
    }

};
