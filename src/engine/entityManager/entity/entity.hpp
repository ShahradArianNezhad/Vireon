#pragma once
#include "./component/components.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <optional>

struct Entity {
  std::optional<TransformComponent> transformComp;
  std::optional<RenderComponent> renderComp;
  unsigned int tint=0xFFFFFF00;


  glm::vec4 tintToVec4(){
    float r =((static_cast<unsigned int>(tint)&0xFF000000)>>24)/255.0f; 
    float g =((static_cast<unsigned int>(tint)&0x00FF0000)>>16)/255.0f; 
    float b =((static_cast<unsigned int>(tint)&0x0000FF00)>>8)/255.0f; 
    float a =((static_cast<unsigned int>(tint)&0x000000FF))/255.0f; 
    return {r,g,b,a};
  }

  glm::vec2 pos(){
    if(transformComp){
      return {transformComp->position.x,transformComp->position.y};
    }
    return {0,0};
  }

  glm::vec2 size(){
    if(transformComp){
      return {transformComp->scale.x,transformComp->scale.y};
    }
    return {0,0};
  }

  void moveBy(float dx,float dy){
    if(transformComp){
      transformComp->position.x+=dx;
      transformComp->position.y+=dy;
    }
  }

  void moveTo(float x,float y){
    if(transformComp){
      transformComp->position.x+=x;
      transformComp->position.y+=y;
    }
  }

  void scale(float amount){
    if(transformComp){
      transformComp->scale.x*=amount;
      transformComp->scale.y*=amount;
    }
  }

  void setTint(unsigned int hex){tint=hex;}
  void setTint(unsigned int r,unsigned int g,unsigned int b,unsigned int a){
    tint=0;
    tint|=((r&0x000000FF)<<24);
    tint|=((g&0x000000FF)<<16);
    tint|=((b&0x000000FF)<<8);
    tint|=(a&0x000000FF);
  }
};

struct Camera : Entity {
  CameraComponent cameraComp;
};
