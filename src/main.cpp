#include "./engine/engine.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include "./game/game.hpp"
#include "engine/eventManager/eventManager.hpp"
#include "platform/input/inputHandler.hpp"
#include "utils/logger/logger.hpp"

class myGame : public Game{
public:
  static constexpr std::string mode = "editor";
  static constexpr int blocksize=32;
  std::vector<EntityId> entities{100*100};
  vec2 uvMin={0,0};
  vec2 uvMax={1.0/10,1.0/10};
  EntityId entity = engine.makeSprite({0,0,0},"./assets/2D Pixel Dungeon Asset Pack/character and tileset/Dungeon_Tileset.png",uvMin,uvMax);
  void init() override {
    engine.setTargetFPS(60);
    if(mode=="editor")setModeEditor();
  }

  void update(double) override {
  };
  



  void setModeEditor(){
    auto render = engine.entityManager.componentManager.getComponent<Component::RENDER>(entity);
    render.color &= 0xFFFFFF99;
    engine.entityManager.componentManager.setComponent(entity, render);
    EventManager::subscribe<MouseMoveEvent>([this ](MouseMoveEvent e){showPreviewAtMousePos({e.x,e.y});});
    EventManager::subscribe<MouseButtonPressedEvent>([this](MouseButtonPressedEvent e){
        if(e.button==Mouse::LEFT)placeAtMousePos();
        else if (e.button==Mouse::RIGHT)deleteAtMousePos();
    });
    EventManager::subscribe<MouseWheelEvent>([this](MouseWheelEvent e){
        if(e.changeY>0)nextTile(entity);
        else prevTile(entity);
    });
    EventManager::subscribe<KeyboardKeyPressedEvent>([this](KeyboardKeyPressedEvent e){
        if(e.key==Key::Left)nextTile(entity);
        else if(e.key==Key::Right)prevTile(entity);
    });
  }

  

  void nextTile(EntityId entity){
    if(uvMax.x<1){
      uvMax.x+=1.0/10;
      uvMin.x+=1.0/10;
    }else{
      uvMax.x=1.0/10;
      uvMin.x=0;
      uvMin.y+=1.0/10;
      uvMax.y+=1.0/10;
    }
    auto rect = engine.entityManager.componentManager.getComponent<Component::UVRECT>(entity);
    rect.uvMin=uvMin;
    rect.uvMax=uvMax;
    engine.entityManager.componentManager.setComponent(entity, rect);
  }
  void prevTile(EntityId entity){
    if(uvMin.x>0){
      uvMax.x-=1.0/10;
      uvMin.x-=1.0/10;
    }else{
      uvMax.x=1.0;
      uvMin.x=9.0/10;
      uvMin.y-=1.0/10;
      uvMax.y-=1.0/10;
    }
    auto rect = engine.entityManager.componentManager.getComponent<Component::UVRECT>(entity);
    rect.uvMin=uvMin;
    rect.uvMax=uvMax;
    engine.entityManager.componentManager.setComponent(entity, rect);
  }
  void placeAtMousePos(){
    auto mousePos = engine.inputHandler.getCursorPos();
    int gridX = std::ceil(mousePos.x/blocksize);
    int gridY = std::ceil(mousePos.y/blocksize);
    vec3 position = {gridX*blocksize - blocksize/2,gridY*blocksize - blocksize/2,0};
    auto id =engine.makeSprite(position,"./assets/2D Pixel Dungeon Asset Pack/character and tileset/Dungeon_Tileset.png",uvMin,uvMax);
    entities[gridX + gridY*100] = id;
  }
  void deleteAtMousePos(){
    auto mousePos = engine.inputHandler.getCursorPos();
    int gridX = std::ceil(mousePos.x/blocksize);
    int gridY = std::ceil(mousePos.y/blocksize);
    engine.entityManager.deleteEntity(entities[gridX + gridY*100]);
  }

  void showPreviewAtMousePos(vec2 pos){
    auto trans = engine.entityManager.componentManager.getComponent<Component::TRANSFORM>(entity);
    int gridX = std::ceil(pos.x/blocksize);
    int gridY = std::ceil(pos.y/blocksize);
    trans.position = {gridX*blocksize - blocksize/2,gridY*blocksize - blocksize/2,0};
    engine.entityManager.componentManager.setComponent(entity, trans);
  }


};

int main() {
  myGame game;
  game.run();
  return 0;
}
