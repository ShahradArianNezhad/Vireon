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
  void reset();
};
