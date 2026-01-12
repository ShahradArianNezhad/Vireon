#pragma once
#include "engine/graphics/elementBuffer/elementBuffer.hpp"
#include "engine/graphics/shader/shader.hpp"
#include "engine/graphics/texture/texture.hpp"
#include "engine/graphics/vertexArray/vertexArray.hpp"
#include "engine/graphics/vertexBuffer/vertexBuffer.hpp"
#include "engine/graphics/vertexLayout/vertexLayout.hpp"
#include <optional>

class Batch {
private:
  Shader shader;
  VertexBuffer vbo;
  std::optional<Texture> texture;
  std::vector<float> vertexData;
  bool changed = false;
  void updatedata();

public:
  VertexLayout layout;
  Batch(Shader &&shader, VertexLayout &&layout);

  Batch(const Batch &) = delete;
  Batch &operator=(const Batch &) = delete;
  Batch(Batch &&other) noexcept;
  void use();
  void addTexture(std::string imagePath);
  void addVertex(std::initializer_list<float> floats);
  int getIndexCount();
};
