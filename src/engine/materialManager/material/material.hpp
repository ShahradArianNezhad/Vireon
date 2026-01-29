#include "./texture/texture.hpp"

class Material {
private:
  Texture texture;

public:
  Material() {};
  Material(std::string texPath) : texture(texPath) {}
  void use() { texture.bind(); }
};
