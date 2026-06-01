#pragma once
#include "glad/gl.h"
#include "utils/logger/logger.hpp"
#include <string>
#include <vector>

class Texture {
private:
  unsigned int ID;

public:
  size_t width,height;
  std::string texturePath="";
  Texture();
  ~Texture(){
    LOG_INFO("deleting texture= {}",ID);
    glDeleteTextures(1,&ID);
  };
  Texture(std::string TexturePath);
  Texture(unsigned int id){ID=id;};
  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;
  Texture(Texture &&other) {
    ID = other.ID;
    other.ID = 0;
  }
  unsigned int getID(){return ID;}
  void uploadAtlas(std::vector<unsigned char>& atlas,size_t w,size_t h);
  void setTex(size_t w,size_t h,void* data);
  void activate(GLenum location);
  void bind();
  void unbind();
};
