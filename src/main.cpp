#include "./engine/engine.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "./game/game.hpp"
#include "engine/entityManager/component/components.hpp"

class myGame : public Game{
public:
  void init() override {
    for(int i=0;i<1'000'00;i++)engine.makeRect({0,0,0}, {100,100});
  }
  void update(double fps) override {
    LOG_INFO("FPS : {}",1/fps);
  };
};

int main() {
  myGame game;
  game.run();
  return 0;
}
