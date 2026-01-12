#pragma once
#include <string>

class Texture {
private:
  unsigned int ID;

public:
  Texture(std::string TexturePath);
  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;
  Texture(Texture &&other) {
    ID = other.ID;
    other.ID = 0;
  }
  void bind();
  void unbind();
};
