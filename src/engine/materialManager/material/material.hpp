#pragma once
#include "./texture/texture.hpp"
#include "utils/logger/logger.hpp"
#include "utils/types.hpp"
#include <format>
#include <optional>

class Material {
public:
  std::optional<Texture> texture;

  Material() {};
  Material(std::string texPath) : texture(texPath) {
    LOG_INFO("Loaded texture : {}",texPath);
  }
  Material(unsigned int tex):texture(tex){};
  void setTexture(std::string imgPath){texture.emplace(Texture{imgPath});}
  void use() { if(texture)texture->bind(); }

  vec2 getTextureDimensions(){
    if(!texture){
      LOG_ERROR("getTextureDims called on Material with no Texture");
      return {0,0};
    }

    return {texture->width,texture->height};
  }
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
