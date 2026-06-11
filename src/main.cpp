#include "./engine/engine.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "./game/game.hpp"
#include "engine/entityManager/component/components.hpp"
#include "engine/eventManager/eventManager.hpp"
#include "platform/window/GLFWwindow.hpp"


struct MoveEvent{
  float dx;
  float dy;
};

class myGame : public Game{
public:
  int counter=0;
  bool face=0;
  EntityId camera=engine.entityManager.makeCamera();
  EntityId c1 = engine.makeSprite({0,0,0},"./assets/Characters(100x100)/Orc/Orc/Orc-Attack01.png",{0,0},{1.0/6,1});

  void init() override {
    for(int i=0;i<100'000;i++)engine.makeRect({0,0,0}, {10,10});
    engine.useCamera(camera, engine.getCurrentScene());
  }


  void update(double) override {
    auto trans = engine.entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(c1);
    if(engine.inputHandler.checkKeyPress(Key::D)) trans.position.x+=10;
    if(engine.inputHandler.checkKeyPress(Key::A)) trans.position.x-=10;
    if(engine.inputHandler.checkKeyPress(Key::W)) trans.position.y-=10;
    if(engine.inputHandler.checkKeyPress(Key::S)) trans.position.y+=10;
    engine.entityManager.componentManager.setComponent<ComponentType::TRANSFORM>(c1, trans);
  };
};

int main() {
  myGame game;
  game.run();
  return 0;
}
