#pragma once
#include <unordered_map>
#include "./scene/scene.hpp"
#include "core/idManager/idManager.hpp"



using SceneId=unsigned int;

class SceneManager{
private:
  std::unordered_map<SceneId, Scene*> scenes;
  IDManager idManager;

public:
  Scene* get(SceneId id);
  SceneId newScene();
};
