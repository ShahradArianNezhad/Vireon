#pragma once
#include "engine/materialManager/material/texture/texture.hpp"
#include "freetype/freetype.h"
#include "glad/gl.h"
#include "utils/allocator/allocator.hpp"
#include "utils/types.hpp"
#include <map>
#include <string>
#include <vector>

struct Character
{
    vec2 uvMin;
    vec2 uvMax;
    int width;
    int height;

    int bearingX;
    int bearingY;

    unsigned int advance;
};

struct GlyphType{
  std::string fontPath;
  size_t fontSize;

  bool operator==(const GlyphType& other) const {
    return fontPath == other.fontPath && fontSize == other.fontSize;
  }
};

namespace std {
    template <>
    struct hash<GlyphType> {
        std::size_t operator()(const GlyphType& p) const {
            std::size_t h1 = std::hash<std::string>{}(p.fontPath);
            std::size_t h2 = std::hash<int>{}(p.fontSize);
            
            return h1 ^ (h2 << 1);
        }
    };
}


class GlyphMap{
  static constexpr size_t ATLAS_WIDTH = 2048;
  static constexpr size_t ATLAS_HEIGHT = 2048;
  FT_Face face;
  FT_Library& ft;
  std::string fontPath;
  size_t fontSize;
  std::vector<unsigned char,EngineAllocator<unsigned char>> atlas;
  Texture tex;

  public:
  std::map<char, Character> characters;
  GlyphMap(FT_Library& ft,GlyphType& gt);
  GLuint getGlyphTex(){return tex.getID();};
  ~GlyphMap(){};

  private:
};
