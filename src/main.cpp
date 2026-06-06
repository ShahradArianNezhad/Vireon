#include "./engine/engine.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "./game/game.hpp"
#include "engine/entityManager/component/components.hpp"

class myGame : public Game{
public:
  int counter=0;
  bool face=0;
  EntityId camera=engine.entityManager.makeCamera();
  EntityId c2 =engine.makeRect({0,0,0},{150,150});
  EntityId c4 =engine.makeCircle({0,0,0},150);
  EntityId c3 =engine.makeRect({-300,0,0},{100,100});

  void init() override {
    auto rect = CircleColliderComponent{150,{0,0}};
    engine.entityManager.componentManager.setComponent<ComponentType::CIRCLECOLLIDER>(c2,rect);
    rect = CircleColliderComponent{100,{0,0}};
    engine.entityManager.componentManager.setComponent<ComponentType::CIRCLECOLLIDER>(c3,rect);
    auto transform = engine.entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(c2);
    engine.entityManager.componentManager.setComponent<ComponentType::TRANSFORM>(c2, transform);
    auto cam = engine.entityManager.componentManager.getComponent<ComponentType::CAMERA2D>(camera);
    engine.entityManager.componentManager.setComponent<ComponentType::CAMERA2D>(camera,cam);
    engine.useCamera(camera, engine.getCurrentScene());
    engine.setTargetFPS(60);

  }


  void update(double) override {
    auto transform = engine.entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(c2);
    if(engine.inputHandler.checkKeyPress(Key::D))transform.position.x+=10;
    if(engine.inputHandler.checkKeyPress(Key::A))transform.position.x-=10;
    if(engine.inputHandler.checkKeyPress(Key::W)) transform.position.y-=10;
    if(engine.inputHandler.checkKeyPress(Key::S)) transform.position.y+=10;



    engine.entityManager.componentManager.setComponent<ComponentType::TRANSFORM>(c2, transform);
  };
};

int main() {
  myGame game;
  game.run();
  return 0;
}
