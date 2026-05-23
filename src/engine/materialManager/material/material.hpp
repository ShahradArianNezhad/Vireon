#include "./texture/texture.hpp"
#include <format>
#include <optional>

class Material {
public:
  std::optional<Texture> texture;

  Material() {};
  Material(std::string texPath) : texture(texPath) {}
  void setTexture(std::string imgPath){texture.emplace(Texture{imgPath});}
  void use() { if(texture)texture->bind(); }
};


template<>
struct std::formatter<Material> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Material& p, std::format_context& ctx) const {
      if(p.texture){
        return std::format_to(
            ctx.out(),
            "{{texture: {}}}",
            (*p.texture).texturePath
        );
    }else return std::format_to(
            ctx.out(),
            "{{No data}}"
        );
    }
};
