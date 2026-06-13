#pragma once
#include "engine/glyphManager/glyphMap/glyphMap.hpp"
#include "glad/gl.h"
#include "utils/allocator/allocator.hpp"
#include <ft2build.h>
#include <string>
#include <unordered_map>
#include FT_FREETYPE_H


class GlyphManager{
  FT_Library ft;
  std::unordered_map<GlyphType,GlyphMap> fontToGlyphMap;

  public:
  GlyphManager();
  GLuint getGlyphTex(std::string fontPath,size_t fontSize);
  Character& getGlyphChar(char c,std::string fontPath,size_t fontSize);
};
