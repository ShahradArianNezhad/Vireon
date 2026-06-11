#include "./engine/engine.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include "./game/game.hpp"
#include "engine/entityManager/component/components.hpp"
#include "platform/window/GLFWwindow.hpp"

class myGame : public Game{
  std::vector<EntityId> circles;
  std::vector<vec2> velocities;
public:
  void init() override {
    engine.window.setFullscreen(0);
    srand(time(0));
    engine.setTargetFPS(60);
    auto count = 100;
    auto segmentW = Screen::width / count;
    auto segmentH = Screen::height / count;
    for(size_t i{};i<count;++i){
      int x = segmentW*i - Screen::width/2.0;
      int y = segmentH*i - Screen::height/2.0;
      auto id = (engine.makeCircle({x,y,0},10));
      circles.push_back(id);
      engine.entityManager.componentManager.setComponent<ComponentType::CIRCLECOLLIDER>(id,CircleColliderComponent{10,{0,0}});
      velocities.push_back(vec2{((rand()%10)-5),((rand()%10)-5)});
    }
  }
  void update(double fps) override {
    for(size_t i=0;i<circles.size();i++){
      for(size_t j=i+1;j<circles.size();j++){
        if(engine.isColliding(circles[i],circles[j])){
          velocities[i]={-velocities[i].x,-velocities[i].y};
          velocities[j]={-velocities[j].x,-velocities[j].y};
        }
      }
    }

    for(size_t i=0;i<circles.size();i++){
      auto trans = engine.entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(circles[i]);
      if(trans.position.x + 10 >= Screen::width/2.0) {      
        velocities[i].x *=-1;
      }
      else if(trans.position.x - 10 <= -(Screen::width/2.0)) {      
        velocities[i].x *=-1;
      }
      if(trans.position.y + 10 >= Screen::height/2.0) {      
        velocities[i].y *=-1;
      }
      else if(trans.position.y - 10 <= -(Screen::height/2.0)) {      
        velocities[i].y *=-1;
      }
    }
    for(size_t i=0;i<circles.size();i++){
      auto trans = engine.entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(circles[i]);
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
