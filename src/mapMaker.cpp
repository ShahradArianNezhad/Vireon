#include "./engine/engine.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <format>
#include <fstream>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include "./game/game.hpp"
#include "engine/entityManager/component/components.hpp"
#include "engine/eventManager/eventManager.hpp"
#include "platform/input/inputHandler.hpp"
#include "json.hpp"
#include "platform/window/GLFWwindow.hpp"




struct Tile{
  EntityId id=UINT32_MAX;
  vec2 uvMin={0,0};
};


struct Entity{
  EntityId id=UINT32_MAX;
  std::string name;
};

using json = nlohmann::json;



class MapEditor : public Game{
public:
  static constexpr int blocksize=32;
  float uvSizeX=10.0;
  float uvSizeY=10.0;
  std::string mode = "tile";
  std::string spriteFile = "./assets/Dungeon_Tileset.png";
  std::unordered_map<long,std::unordered_map<long,std::vector<Tile>>> tileMap;
  std::unordered_map<long,std::unordered_map<long,Entity>> entityMap;
  std::unordered_map<long,std::unordered_map<long,EntityId>> lightMap;

  vec2 uvMin={0,0};
  vec2 uvMax={1.0/10,1.0/10};
  EntityId selection = engine.makeSprite({0,0,10},spriteFile,uvMin,uvMax);
  EntityId selectionBox = engine.makeRect({0,0,20}, {200,200},Layer::UI);
  std::vector<EntityId> selectionItems;





  void init() override {
    engine.setAmbient(0.5);
    engine.setTargetFPS(60);
    setModeEditor();
  }

  void update(double) override {
    auto cam = engine.getActiveCamera();
    auto camComp = engine.entityManager.componentManager.getComponent<Component::CAMERA2D>(cam);
    if(engine.inputHandler.checkKeyPress(Key::Up)){camComp.position.y-=5;}
    if(engine.inputHandler.checkKeyPress(Key::Down)){camComp.position.y+=5;}
    if(engine.inputHandler.checkKeyPress(Key::Left)){camComp.position.x-=5;}
    if(engine.inputHandler.checkKeyPress(Key::Right)){camComp.position.x+=5;}
    engine.entityManager.componentManager.setComponent(cam, camComp);
  };
  



  void setModeEditor(){
    auto render = engine.entityManager.componentManager.getComponent<Component::RENDER>(selection);
    render.color &= 0xFFFFFF55;
    engine.entityManager.componentManager.setComponent(selection, render);

    EventManager::subscribe<MouseMoveEvent>([this](MouseMoveEvent e){this->showPreviewAtMousePos({e.x,e.y});});
    EventManager::subscribe<MouseButtonPressedEvent>([this](MouseButtonPressedEvent e){
        if(e.button==Mouse::LEFT && mode!="select")placeAtMousePos();
        else if(e.button==Mouse::LEFT && mode=="select")selectTile();
        else if (e.button==Mouse::RIGHT)deleteAtMousePos();
    });
    EventManager::subscribe<MouseWheelEvent>([this](MouseWheelEvent e){
        if(e.changeY>0)nextTile(selection);
        else prevTile(selection);
    });
    EventManager::subscribe<KeyboardKeyPressedEvent>([this](KeyboardKeyPressedEvent e){
        if(e.key==Key::W)writeTileMap();
        else if(e.key==Key::R)loadTileMap();
        else if(e.key==Key::E)nextSelection();
        else if(e.key==Key::Tab && mode=="tile")openSelectMenu();
        else if(e.key==Key::Tab && mode=="select")closeSelectMenu();
    });
    engine.changeColor(selectionBox, 0x808080FF);

    for(int j=0;j<10;j++){
      for(int i=0;i<10;i++){
        selectionItems.push_back(engine.makeSprite({((i+1)*40)-220 ,((j+1)*40)-220,21},"./assets/Dungeon_Tileset.png",{i/10.0,j/10.0},{(i+1)/10.0,(j+1)/10.0},Layer::UI));
      }
    }
    closeSelectMenu();
  }

  

  void selectTile(){
    auto cursorPos = engine.inputHandler.getCursorPos();
    if(cursorPos.x >200 || cursorPos.x <-200 || cursorPos.y >200 || cursorPos.y <-200)return;
    uvMin.x = (std::ceil(cursorPos.x/40)+4)/10.0;
    uvMin.y = (std::ceill(cursorPos.y/40)+4)/10.0;
    uvMax.x = (std::ceil(cursorPos.x/40)+5)/10.0;
    uvMax.y = (std::ceil(cursorPos.y/40)+5)/10.0;
    engine.changeSprite(selection,"./assets/Dungeon_Tileset.png",uvMin,uvMax);
    closeSelectMenu();
  }



  void openSelectMenu(){
    for(auto e:selectionItems)engine.setVisibility(e,true);
    engine.setVisibility(selectionBox, true);
    mode="select";
    
  }


  void closeSelectMenu(){
    for(auto e:selectionItems)engine.setVisibility(e,false);
    engine.setVisibility(selectionBox, false);
    mode="tile";
  }

  

  void nextSelection(){
    if(mode=="tile"){
      uvSizeX = 6;
      uvSizeY = 1;
      spriteFile="./assets/skeleton/skeleton2_idle.png";
      mode = "skeleton";
    }else if(mode=="skeleton"){
      uvSizeX = 6;
      uvSizeY = 1;
      spriteFile = "./assets/Soldier/Soldier-Idle.png";
      mode = "soldier";
    }else if(mode=="soldier"){
      uvSizeX = 6;
      uvSizeY = 1;
      spriteFile = "./assets/vampire/vampire_idle.png";
      mode = "vampire";
    }else if(mode=="vampire"){
      uvSizeX = 10;
      uvSizeY = 10;
      spriteFile = "./assets/Dungeon_Tileset.png";
      mode = "tile";
    }
    uvMin= {0,0};
    uvMax= {1.0/uvSizeX,1.0/uvSizeY};
    engine.changeSprite(selection,spriteFile,uvMin,uvMax);
  };

  void clearScreen(){
    for(auto& [gridX,gridYtoTileVec]:tileMap){
      for(auto& [gridY,tileVec]:tileMap[gridX]){
        for(auto& tile:tileMap[gridX][gridY]){
          engine.entityManager.deleteEntity(tile.id);
        }
      }
    }
    for(auto& [gridX,gridYtoEntity]:entityMap){
      for(auto& [gridY,entity]:gridYtoEntity){
        engine.entityManager.deleteEntity(entity.id);
      }
    }
    for(auto& [gridX,gridYtoLight]:lightMap){
      for(auto& [gridY,light]:gridYtoLight){
        engine.entityManager.deleteEntity(light);
      }
    }
    tileMap.clear();
    entityMap.clear();
    lightMap.clear();
  }

  void loadTileMap(){
    clearScreen();
    std::ifstream file;
    file.open("map.json");
    json data = json::parse(file);
    for(auto& [tileX,tileYtoUv]:data["tile"].items()){
      for(auto& [tileY,uvArray]:tileYtoUv.items()){
        size_t z=0;
        for(std::string uvText:uvArray){
          vec2 uv;
          std::string x{uvText.begin()+1,uvText.begin()+uvText.find(",")};
          std::string y{uvText.begin()+uvText.find(",")+1,uvText.end()-1};
          uv.x = std::stof(x);
          uv.y = std::stof(y);
          auto gridX = std::stoi(tileX);
          auto gridY = std::stoi(tileY);
          vec3 position = {gridX*blocksize - blocksize/2,gridY*blocksize - blocksize/2,z++};
          auto id =engine.makeSprite(position,"./assets/Dungeon_Tileset.png",uv,{uv.x+1/10.0,uv.y+1/10.0});
          tileMap[gridX][gridY].emplace_back(id,uv);
        }
      }
    }
    for(auto& [tileX,tileYtoName]:data["entity"].items()){
      for(auto& [tileY,name]:tileYtoName.items()){
        EntityId id;
        auto gridX = std::stoi(tileX);
        auto gridY = std::stoi(tileY);
        vec3 position = {gridX*blocksize - blocksize/2,gridY*blocksize - blocksize/2,10};
        if(name=="skeleton")     id =engine.makeSprite(position,"./assets/skeleton/skeleton2_idle.png",{0,0},{1.0/6,1.0});
        else if(name=="vampire") id =engine.makeSprite(position,"./assets/vampire/vampire_idle.png",{0,0},{1.0/6,1.0});
        else if(name=="soldier") id =engine.makeSprite(position,"./assets/Soldier/Soldier-Idle.png",{0,0},{1.0/6,1.0});
        else{
          LOG_ERROR("INVALID NAME IN ENTITY LIST");
          return;
        }
        entityMap[gridX][gridY]=Entity{id,name};
      }
    }
    for(auto& [tileX,tileYtoLight]:data["light"].items()){
      for(auto& [tileY,light]:tileYtoLight.items()){
        auto gridX = std::stoi(tileX);
        auto gridY = std::stoi(tileY);
        vec3 position = {gridX*blocksize - blocksize/2,gridY*blocksize - blocksize/2,10};
        auto id = engine.makeLight(position,{255,255,255},200,0.004);
        lightMap[gridX][gridY]=id;
      }
    }

  }

  void writeTileMap(){
    std::ofstream file;
    json map;
    file.open("map.json");
    for(auto& [gridX,gridYtoVec]:tileMap){
      for(auto& [gridY,tileVec]:gridYtoVec){
        for(auto& tile:tileVec){
          map["tile"][std::format("{}",gridX)][std::format("{}",gridY)].push_back(std::format("{{{},{}}}",tile.uvMin.x,tile.uvMin.y));
        }
      }
    }
    for(auto& [gridX,gridYtoEntity]:entityMap){
      for(auto& [gridY,entity]:gridYtoEntity){
        map["entity"][std::format("{}",gridX)][std::format("{}",gridY)] = entity.name;
      }
    }
    for(auto& [gridX,gridYtoLight]:lightMap){
      for(auto& [gridY,light]:gridYtoLight){
        map["light"][std::format("{}",gridX)][std::format("{}",gridY)] = light;
      }
    }

    file << std::setw(4) << map << std::endl;
    file.flush();
    file.close();
  };

  

  void nextTile(EntityId entity){
    if(uvMax.x<1){
      uvMax.x+=1.0/uvSizeX;
      uvMin.x+=1.0/uvSizeX;
    }else{
      uvMax.x=1.0/uvSizeX;
      uvMin.x=0;
      uvMin.y+=1.0/uvSizeY;
      uvMax.y+=1.0/uvSizeY;
    }
    auto rect = engine.entityManager.componentManager.getComponent<Component::UVRECT>(entity);
    rect.uvMin=uvMin;
    rect.uvMax=uvMax;
    engine.entityManager.componentManager.setComponent(entity, rect);
  }
  void prevTile(EntityId entity){
    if(uvMin.x>0){
      uvMax.x-=1.0/uvSizeX;
      uvMin.x-=1.0/uvSizeX;
    }else{
      uvMax.x=1.0;
      uvMin.x=9.0/uvSizeX;
      uvMin.y-=1.0/uvSizeY;
      uvMax.y-=1.0/uvSizeY;
    }
    auto rect = engine.entityManager.componentManager.getComponent<Component::UVRECT>(entity);
    rect.uvMin=uvMin;
    rect.uvMax=uvMax;
    engine.entityManager.componentManager.setComponent(entity, rect);
  }
  void placeAtMousePos(){
    if(mode=="select"){
      return;
    };
    auto cam = engine.entityManager.componentManager.getComponent<Component::CAMERA2D>(engine.getActiveCamera());
    auto mousePos = engine.inputHandler.getCursorPos();
    int gridX = std::ceil((mousePos.x+cam.position.x)/blocksize);
    int gridY = std::ceil((mousePos.y+cam.position.y)/blocksize);
    if(mode!="tile" && entityMap[gridX].contains(gridY)) return;
    size_t z;
    if(mode=="tile") z = tileMap[gridX][gridY].size(); 
    else z = 10;
    vec3 position = {gridX*blocksize - blocksize/2,gridY*blocksize - blocksize/2,z};
    auto id =engine.makeSprite(position,spriteFile,uvMin,uvMax);
    if(mode=="tile" && uvMax.y==1 && !lightMap[gridX].contains(gridY)){
      auto lightId = engine.makeLight(position,{255,255,255},200,0.004);
      lightMap[gridX][gridY]=lightId;
    }
    if(mode=="tile")tileMap[gridX][gridY].emplace_back(id,uvMin);
    else entityMap[gridX][gridY]=Entity{id,mode};
  }
  void deleteAtMousePos(){
    if(mode=="select")return;
    auto cam = engine.entityManager.componentManager.getComponent<Component::CAMERA2D>(engine.getActiveCamera());
    auto mousePos = engine.inputHandler.getCursorPos();
    int gridX = std::ceil((mousePos.x+cam.position.x)/blocksize);
    int gridY = std::ceil((mousePos.y+cam.position.y)/blocksize);
    if(entityMap[gridX].contains(gridY)){
      engine.entityManager.deleteEntity(entityMap[gridX][gridY].id);
      entityMap[gridX].erase(gridY);
    }else{
      if(tileMap[gridX][gridY].size()==0)return;
      if(lightMap[gridX].contains(gridY)){
        engine.entityManager.deleteEntity(lightMap[gridX][gridY]);
        lightMap[gridX].erase(gridY);
      }
      engine.entityManager.deleteEntity(tileMap[gridX][gridY].back().id);
      tileMap[gridX][gridY].pop_back();
    }
  }

  void showPreviewAtMousePos(vec2 pos){
    auto trans = engine.entityManager.componentManager.getComponent<Component::TRANSFORM>(selection);
    auto cam = engine.entityManager.componentManager.getComponent<Component::CAMERA2D>(engine.getActiveCamera());
    int gridX = std::ceil(pos.x/blocksize);
    int gridY = std::ceil(pos.y/blocksize);
    trans.position = {gridX*blocksize - blocksize/2.0 + cam.position.x,gridY*blocksize - blocksize/2.0 + cam.position.y,10};
    engine.entityManager.componentManager.setComponent(selection, trans);
  }

};

int main() {
  MapEditor game;
  game.run();
  return 0;
}
