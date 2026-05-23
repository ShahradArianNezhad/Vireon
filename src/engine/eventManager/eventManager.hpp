#pragma once
#include "engine/entityManager/component/components.hpp"
#include "utils/idManager/idManager.hpp"

struct EntityDestroyedEvent{
  EntityId id;
};


using subscriptionId = uint32_t;

class EventManager{

public:
  using Handler = std::function<void(const EntityDestroyedEvent&)>;
  
   subscriptionId subscribe(Handler handler){
    auto id = idManager.get();
    if(id>=listeners.size()){
      listeners.push_back(handler);
    }else{
      listeners[id]=handler;
    }
    LOG_DEBUG("subscribed to entityDestroyedEvent emitter: {}",id);
    return id;
  }

  void unsubscribe(subscriptionId id){
    LOG_DEBUG("unsubscribe from entityDestroyedEvent emitter: {}",id);
    listeners[id]=nullptr;
  }

  void emit(const EntityDestroyedEvent& event){
    LOG_DEBUG("emit entityDestroyedEvent");
    for(auto& listener:listeners){
      if(listener!=nullptr){
        listener(event);
      };

    }
  }


private:
  std::vector<Handler> listeners;
  IDManager idManager;


};
