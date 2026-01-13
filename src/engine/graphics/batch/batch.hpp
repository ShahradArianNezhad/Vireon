#pragma once
#include "engine/graphics/elementBuffer/elementBuffer.hpp"
#include "engine/graphics/object/object.hpp"
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
  VertexArray vao;
  std::optional<ElementBuffer> ebo;
  std::optional<Texture> texture;
  std::vector<Object> objects;
  bool changed = false;
  void updatedata();

public:
  VertexLayout layout;
  Batch(Shader &&shader, VertexLayout layout);

  Batch(const Batch &) = delete;
  Batch &operator=(const Batch &) = delete;
  Batch(Batch &&other) noexcept
      : shader(std::move(other.shader)), vbo(std::move(other.vbo)),
        vao(std::move(other.vao)), ebo(std::move(other.ebo)),
        texture(std::move(other.texture)), objects(std::move(other.objects)),
        changed(other.changed), layout(std::move(other.layout)) {}

  int getIndicesCount();

  bool isUsingEbo() {
    if (ebo) {
      return true;
    } else {
      return false;
    }
  }

  void use();
  void submit(Object obj);
  void addTexture(std::string imagePath);
};
