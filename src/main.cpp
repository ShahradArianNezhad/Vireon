#include "./engine/engine.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include "./game/game.hpp"
#include "engine/entityManager/component/components.hpp"
#include "platform/window/GLFWwindow.hpp"

class myGame : public Game{
  static constexpr int count = 2000;
  static constexpr int radius = 2;
  EntityId circles[count];
  vec2 velocities[count];
public:
  void init() override {
    //engine.window.setFullscreen(0);
    //engine.setTargetFPS(60);
    srand(time(0));
    for(int i{0};i<count;++i){
      int x = (i*(radius+15) % 1900) - 1900/2;
      int y = (i*(radius+15) / 1900)*15 - 1000/2;
      auto id = (engine.makeCircle({x,y,0},radius));
      circles[i]= id;
      engine.entityManager.componentManager.setComponent<ComponentType::CIRCLECOLLIDER>(id,CircleColliderComponent{radius,{0,0}});
      velocities[i] = vec2{((rand()%10)-5),((rand()%10)-5)};
    }
  }
  void update(double fps) override {
    LOG_INFO("fps : {}",1/fps);
    for(size_t i=0;i<count;i++){
      auto trans = engine.entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(circles[i]);
      if(trans.position.x + 10 >= 1920/2.0) {      
        velocities[i].x *=-1;
      }
      else if(trans.position.x - 10 <= -(1920/2.0)) {      
        velocities[i].x *=-1;
      }
      if(trans.position.y + 10 >= 1080/2.0) {      
        velocities[i].y *=-1;
      }
      else if(trans.position.y - 10 <= -(1080/2.0)) {      
        velocities[i].y *=-1;
      }
      for(size_t j=i+1;j<count;j++){
        if(engine.circleIsColliding(circles[i],circles[j])){
          velocities[i]={-velocities[i].x,-velocities[i].y};
          velocities[j]={-velocities[j].x,-velocities[j].y};
        }
      }
      trans.position.x+=velocities[i].x;
      trans.position.y+=velocities[i].y;
      engine.entityManager.componentManager.setComponent<ComponentType::TRANSFORM>(circles[i], trans);
    }
  };
};

int main() {
  myGame game;
  game.run();
  return 0;
}
