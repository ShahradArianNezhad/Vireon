#include "meshManager.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"

MeshManager::MeshManager() {};

MeshID MeshManager::makePrimitive(Primitive shape, Color color) {
  float red = ((static_cast<unsigned int>(color) & 0xFF0000) >> 16);
  float green = ((static_cast<unsigned int>(color) & 0x00FF00) >> 8);
  float blue = (static_cast<unsigned int>(color) & 0x0000FF);
  red /= 255;
  green /= 255;
  blue /= 255;

  switch (shape) {
  case Primitive::Triangle: {
    std::vector<Vertex> v = {
        Vertex{{0.5f, -0.5f, 0.0f}, {red, green, blue}},  //
        Vertex{{-0.5f, -0.5f, 0.0f}, {red, green, blue}}, //
        Vertex{{0.0f, 0.5f, 0.0f}, {red, green, blue}},   //
    };
    std::vector<unsigned int> i = {0, 1, 2};
    Mesh mesh{v, i, VertexLayout::PosColor};
    return submit(mesh);
  }

  case Primitive::Square: {
    std::vector<Vertex> v = {
        Vertex{{0.5f, -0.5f, 0.0f}, {red, green, blue}},  // bottom-right
        Vertex{{-0.5f, -0.5f, 0.0f}, {red, green, blue}}, // bottom-left
        Vertex{{0.5f, 0.5f, 0.0f}, {red, green, blue}},   // top-right
        Vertex{{-0.5f, 0.5f, 0.0f}, {red, green, blue}},  // top-left
    };
    std::vector<unsigned int> i = {
        0, 1, 2, //
        3, 2, 1  //
    };
    Mesh mesh{v, i, VertexLayout::PosColor};
    return submit(mesh);
  }
  }
};
