#include "./texture/texture.hpp"
#include "engine/meshManager/meshManager.hpp"
#include <optional>

class Material {
public:
  std::optional<Texture> texture;
  std::optional<Color> color;

  Material() {};
  Material(std::string texPath) : texture(texPath) {}
  Material(Color iColor) : color(iColor) {}
  glm::vec4 colorToVec4();
  void use() { if(texture)texture->bind(); }
};
