#pragma once
#include "engine/entityManager/component/components.hpp"
#include "utils/idManager/idManager.hpp"
#include "utils/objectPool/objectPool.hpp"

using subscriptionId = uint32_t;

class EventManager{

public:
  template<typename T>
    struct Handler{
      subscriptionId id;
      std::function<void(const T&)> func;
    };
  
  template<typename T>
   static inline subscriptionId subscribe(std::function<void(const T&)> handler){
      LOG_DEBUG("subscribed to event :{}",typeid(T).name());
      auto id = ids<T>.get();
      handlerPool<T>.add(Handler{id,handler});
      return id;
    }

  template<typename T>
    static inline void unsubscribe(subscriptionId id){
      LOG_DEBUG("unsubscribe from event: {}, with subscriptionId={}",typeid(T).name(),id);
      ids<T>.release(id);
      handlerPool<T>.remove(id);
    }

  template<typename T>
    static inline void emit(const T& event){
      LOG_DEBUG("emit event:{}",typeid(T).name());
      for(const auto& listener:handlerPool<T>.pool) listener.func(event);
      }


private:
  template<typename T>
    static inline ObjectPool<Handler<T>> handlerPool;

  template<typename T>
    static inline IDManager ids;


};
