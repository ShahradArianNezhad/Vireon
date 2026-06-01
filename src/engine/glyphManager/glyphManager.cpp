#include "./glyphManager.hpp"
#include "utils/logger/logger.hpp"

GlyphManager::GlyphManager(){
  if (FT_Init_FreeType(&ft)) LOG_ERROR("failed to init freetype");
}


GLuint GlyphManager::getGlyphTex(std::string fontPath,size_t fontSize){
  GlyphType glyph{fontPath,fontSize};
  auto [it, inserted] = fontToGlyphMap.try_emplace(glyph,ft, glyph);

  return it->second.getGlyphTex();
}



Character& GlyphManager::getGlyphChar(char c,std::string fontPath,size_t fontSize){
  GlyphType glyph{fontPath,fontSize};
  auto [it, inserted] = fontToGlyphMap.try_emplace(glyph,ft, glyph);

  return it->second.characters[c];
}
