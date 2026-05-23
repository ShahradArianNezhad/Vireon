#pragma once
#include "engine/eventManager/eventManager.hpp"
#include "utils/logger/logger.hpp"
#include <cstdint>
#include <vector>

using SceneId = uint32_t;

class Scene {
private:
  SceneId id;
  std::vector<EntityId> entities;
  std::vector<uint32_t> empty_slots;
  EntityId camera=UINT32_MAX;
  EventManager& eventManager;
  subscriptionId subId;

public:
  Scene(SceneId id,EventManager& eManager):id(id),eventManager(eManager){
    subId = eventManager.subscribe(
        [this](const EntityDestroyedEvent& e) {
        entityDestroyedEventHandler(e);
        });
  };
  ~Scene(){
    LOG_DEBUG("scene destructed id:{}",id);
    eventManager.unsubscribe(subId);
  }
  std::vector<EntityId> &collectEntities() {return entities;}

  void addEntity(EntityId entity) {
    LOG_DEBUG("entity:{} added to scene:{}",entity,id);
    if(empty_slots.empty())entities.push_back(entity);
    else {
      entities[empty_slots.back()]=entity;
      empty_slots.pop_back();
    }

  };
  void entityDestroyedEventHandler(const EntityDestroyedEvent& event){
    LOG_DEBUG("entity:{} removed from scene:{}",event.id,id);
    for(size_t i=0;i<entities.size();i++){
      if(entities[i]==event.id){
        entities[i]=UINT32_MAX;
        empty_slots.push_back(i);
        break;
      }
    }
  }

  void setActiveCamera(EntityId cam){
    LOG_DEBUG("camera:{} set active in scene:{}",cam,id);
    camera=cam;
  }
  EntityId getActiveCamera(){
    return camera;
  }
};
