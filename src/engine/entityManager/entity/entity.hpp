#pragma once
#include "./component/components.hpp"
#include <glm/glm.hpp>
#include <optional>

struct Entity {
  std::optional<TransformComponent> transformComp;
  std::optional<RenderComponent> renderComp;

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
};

struct Camera : Entity {
  CameraComponent cameraComp;
};
