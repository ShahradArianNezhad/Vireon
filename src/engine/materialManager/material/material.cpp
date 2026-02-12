#include "./material.hpp"
#include <stdexcept>


glm::vec4 Material::colorToVec4(){
  if(!color) throw std::runtime_error{"colorToVec4 called on material with no color"};
  float r =((static_cast<unsigned int>(*color)&0xFF000000)>>24)/255.0f; 
  float g =((static_cast<unsigned int>(*color)&0x00FF0000)>>16)/255.0f; 
  float b =((static_cast<unsigned int>(*color)&0x0000FF00)>>8)/255.0f; 
  float a =((static_cast<unsigned int>(*color)&0x000000FF))/255.0f; 
  return {r,g,b,a};
}
