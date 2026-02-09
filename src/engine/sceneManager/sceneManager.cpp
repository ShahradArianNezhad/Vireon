#include "./sceneManager.hpp"
#include <memory>
#include <stdexcept>


SceneId SceneManager::newScene(){
  auto id = idManager.get();
  scenes.emplace(id,new Scene);
  return id;
}


Scene* SceneManager::get(SceneId id){
  if(scenes.contains(id)){
    return scenes[id];
  }
  throw std::runtime_error("tried to access an invalid sceneId");
  return nullptr;
}
