#include "./glyphMap.hpp"
#include "freetype/freetype.h"
#include "utils/logger/logger.hpp"
#include <algorithm>
#include <filesystem>

GlyphMap::GlyphMap(FT_Library& ft,GlyphType& gt):ft(ft),fontPath(gt.fontPath),fontSize(gt.fontSize){
  int penX{},penY{},rowHeight{};
  atlas.resize(ATLAS_WIDTH * ATLAS_HEIGHT);
  const char* c_str = std::filesystem::path(fontPath).c_str();
  if (FT_New_Face(ft, c_str, 0, &face)){
    LOG_ERROR("failed to load font:{}",fontPath);
    return ;
  }
  if (FT_Set_Pixel_Sizes(face, 0, fontSize)) {
    LOG_ERROR("failed to set font size: {}", fontSize);
    return;
  }

  for(unsigned char c=0;c<128;c++){
    if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
      LOG_WARN("failed to load char={}",c);
      continue;
    }
    auto& bitmap = face->glyph->bitmap;
    if(penX + bitmap.width>= ATLAS_WIDTH){
      penX=0;
      penY+=rowHeight;
      rowHeight=0;
    }
    for(size_t y=0;y<bitmap.rows;y++){
      for(size_t x=0;x<bitmap.width;x++){
        atlas[(penY+y) * ATLAS_WIDTH + (penX+x)] = bitmap.buffer[y*bitmap.pitch+x];
      }
    }
    vec2 uvMin,uvMax;
    uvMin.x= (float)penX / ATLAS_WIDTH;
    uvMin.y= (float)penY / ATLAS_HEIGHT;
    uvMax.x= (float)(penX+bitmap.width)/ATLAS_WIDTH;
    uvMax.y= (float)(penY+bitmap.rows)/ATLAS_HEIGHT;
    characters.try_emplace(c,uvMin,uvMax,bitmap.width,bitmap.rows,face->glyph->bitmap_left,face->glyph->bitmap_top,face->glyph->advance.x >> 6);
    penX+= bitmap.width+1;
    rowHeight= std::max(rowHeight,int(bitmap.rows));
  }
  tex.uploadAtlas(atlas, ATLAS_WIDTH, ATLAS_HEIGHT);
};






