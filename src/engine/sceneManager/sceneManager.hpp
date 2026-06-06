#pragma once
#include <unordered_map>
#include "./scene/scene.hpp"
#include "engine/eventManager/eventManager.hpp"
#include "utils/idManager/idManager.hpp"



using SceneId=unsigned int;

class SceneManager{
private:
  std::unordered_map<SceneId, Scene> scenes;
  IDManager idManager;

public:
  SceneManager(){};
  Scene* get(SceneId id);
  SceneId newScene();
};
