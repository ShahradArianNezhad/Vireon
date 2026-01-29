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
  scene.addEntity(makeTriangle());

  while (!window.windowShouldClose()) {
    renderer.flush();
    renderer.renderScene(scene);
    window.updateWindow();
  }
}

Entity Engine::makeTriangle() {
  std::vector<Vertex> v = {
      Vertex{{1.0, 1.0, 0.0}, {1.0, 0.0, 0.0}},  //
      Vertex{{-1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}}, //
      Vertex{{1.0, -1.0, 0.0}, {1.0, 0.0, 0.0}}, //
      Vertex{{1.0, -1.0, 0.0}, {1.0, 0.0, 0.0}}, //
      Vertex{{-1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}}, //
      Vertex{{-1.0, -1.0, 0.0}, {0.0, 1.0, 0.0}} //
  };
  std::vector<unsigned int> indices = {0, 1, 2, 3, 4, 5};
  Mesh mesh{v, indices, VertexLayout::PosColor};
  // new one each time (BAD)
  auto id = meshManager.submit(mesh);
  auto matId = materialManager.submit(Material{"./container.jpg"});
  Entity entity{id, matId, true};
  return entity;
}
