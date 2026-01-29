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
  void reset();
};
