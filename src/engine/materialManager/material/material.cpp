#include "./material.hpp"
#include <stdexcept>


glm::vec4 Material::colorToVec4(){
  if(!color) throw std::runtime_error{"colorToVec4 called on material with no color"};
  unsigned int r =(static_cast<unsigned int>(*color)&0xFF000000)>>24; 
  unsigned int g =(static_cast<unsigned int>(*color)&0x00FF0000)>>16; 
  unsigned int b =(static_cast<unsigned int>(*color)&0x0000FF00)>>8; 
  unsigned int a =(static_cast<unsigned int>(*color)&0x000000FF); 
  return {r,g,b,a};
}
