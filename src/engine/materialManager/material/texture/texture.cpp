#include "utils/allocator/allocator.hpp"
#include "utils/logger/logger.hpp"
#include "texture.hpp"
#include "glad/gl.h"
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string TexturePath) {
    texturePath = std::filesystem::path(TexturePath).string();
    glGenTextures(1, &ID);
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //stbi_set_flip_vertically_on_load(true);

    int w,h,nrChannels;
    unsigned char* data =
        stbi_load(TexturePath.c_str(), &w, &h,
                  &nrChannels, STBI_rgb_alpha);

    if (data) {
        GLenum format = GL_RGB;
        width=w;
        height=h;

        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     format,
                     w,
                     h,
                     0,
                     format,
                     GL_UNSIGNED_BYTE,
                     data);

        glGenerateMipmap(GL_TEXTURE_2D);

        LOG_DEBUG("Loaded texture {}", texturePath);
    } else {
        LOG_ERROR("Failed to load texture {}", texturePath);
    }

    stbi_image_free(data);
}


Texture::Texture(){
  glGenTextures(1, &ID);
}


void Texture::uploadAtlas(std::vector<unsigned char,EngineAllocator<unsigned char>>& atlas,size_t w,size_t h){

  bind();
  width=w;
  height=h;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_R8,
      w,
      h,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      atlas.data());

  glActiveTexture(GL_TEXTURE0);
}

void Texture::activate(GLenum location){
  glActiveTexture(location);
  bind();
}

void Texture::bind() {
  glBindTexture(GL_TEXTURE_2D, ID); 
#ifdef DEBUG_VERBOSE
  LOG_DEBUG("bound texture:{}",ID);
#endif
}
void Texture::unbind() {
  glBindTexture(GL_TEXTURE_2D, 0); 
#ifdef DEBUG_VERBOSE
  LOG_DEBUG("unbound texture:{}",ID);
#endif
}

void Texture::setTex(size_t w,size_t h,void* data){
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
}
