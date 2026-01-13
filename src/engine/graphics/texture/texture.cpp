#define STB_IMAGE_IMPLEMENTATION
#include "texture.hpp"
#include "glad/gl.h"
#include "stb_image.h"

Texture::Texture(std::string TexturePath) {
  texturePath = TexturePath;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load(TexturePath.c_str(), &width, &height, &nrChannels, 0);
  glGenTextures(1, &ID);
  bind();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
}
void Texture::bind() { glBindTexture(GL_TEXTURE_2D, ID); }
void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
