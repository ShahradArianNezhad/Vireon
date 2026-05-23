#include "./sceneManager.hpp"
#include "utils/logger/logger.hpp"
#include <stdexcept>


SceneId SceneManager::newScene(){
  auto id = idManager.get();
  scenes.try_emplace(id,id,eventManager);
  LOG_DEBUG("Scene created id:{}",id);
  return id;
}


Scene* SceneManager::get(SceneId id) {
    auto it = scenes.find(id);
    if (it == scenes.end()){ 
      LOG_ERROR("get called on scene with an invalid sceneId:{}",id);
      return nullptr;
    }

    return &it->second;
}
