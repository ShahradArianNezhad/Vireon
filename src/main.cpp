#include "./engine/engine.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "./game/game.hpp"
#include "engine/entityManager/component/components.hpp"
#include "engine/eventManager/eventManager.hpp"


struct MoveEvent{
  float dx;
  float dy;
};

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
    engine.entityManager.deleteEntity(c4);

    EventManager::subscribe<MoveEvent>([this](const MoveEvent& e){
    auto transform = engine.entityManager.componentManager.getComponent<ComponentType::TRANSFORM>(c2);
    transform.position.x+=e.dx;
    transform.position.y+=e.dy;
    engine.entityManager.componentManager.setComponent<ComponentType::TRANSFORM>(c2, transform);
        });

  }


  void update(double) override {
    if(engine.inputHandler.checkKeyPress(Key::D))EventManager::emit(MoveEvent{10,0});
    if(engine.inputHandler.checkKeyPress(Key::A))EventManager::emit(MoveEvent{-10,0});
    if(engine.inputHandler.checkKeyPress(Key::W))EventManager::emit(MoveEvent{0,-10});
    if(engine.inputHandler.checkKeyPress(Key::S))EventManager::emit(MoveEvent{0,10});



  };
};

int main() {
  myGame game;
  game.run();
  return 0;
}
