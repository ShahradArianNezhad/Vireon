#include "./engine.hpp"
#include "engine/graphics/vertex/vertex.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include "engine/meshManager/meshManager.hpp"
#include "engine/scene/entity/entity.hpp"
#include "glad/gl.h"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

void Engine::mainloop() {
  Scene scene;
  auto triangle = makeTriangle();
  scene.addEntity(triangle);

  while (!window.windowShouldClose()) {
    renderer.flush();
    renderer.collectAndBatch(scene);
    renderer.renderBatches();
    window.updateWindow();
  }
}

Entity Engine::makeTriangle() {
  std::vector<Vertex> v = {
      Vertex{{-1.0, -1.0, 0.0}, {1.0, 0.0, 0.0}}, //
      Vertex{{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}},   //
      Vertex{{1.0, -1.0, 0.0}, {0.0, 0.0, 1.0}}   //
  };
  std::vector<unsigned int> indices = {0, 1, 2};
  Mesh mesh{v, indices, VertexLayout::PosColor};
  auto id = meshManager.submit(mesh);
  Entity entity{id, true};
  return entity;
}
