#include "./engine/engine.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "./game/game.hpp"
#include "engine/entityManager/component/components.hpp"

class myGame : public Game{
public:
  EntityId camera=engine.entityManager.makeCamera();
  void init() override {
    engine.useCamera(camera, engine.getCurrentScene());
    for(int i=0;i<1'000;i++)engine.makeText("a", {0,0,0}, "./assets/Arial.ttf", 64);
  }
  void update(double fps) override {
  };
};

int main() {
  myGame game;
  game.run();
  return 0;
}
