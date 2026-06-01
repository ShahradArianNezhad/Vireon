#include "./scheduleManager.hpp"
#include "utils/logger/logger.hpp"
#include "utils/objectPool/objectPool.hpp"


TaskId ScheduleManager::do_after(float secs,std::function<void()> callback){
  auto id = idManager.get();
  Task task{.id=id,.remainingTime=secs,.interval=secs,.repeating=false,.callback=callback};
  taskPool.add(task);
  LOG_DEBUG("do after:{} taskId:{}",secs,id);
  return id;
} 

TaskId ScheduleManager::do_every(float secs,std::function<void()> callback){
  auto id = idManager.get();
  Task task{.id=id,.remainingTime=secs,.interval=secs,.repeating=true,.callback=callback};
  taskPool.add(task);
  LOG_DEBUG("do every:{} taskId:{}",secs,id);
  return id;
} 

void ScheduleManager::cancel_task(TaskId id){
  taskPool.remove(id);
  LOG_DEBUG("task removed taskId:{}",id);
}


void ScheduleManager::update(float dt){
  for(auto& task:taskPool.pool){
    task.remainingTime-=dt;
    if(task.remainingTime<=0.0f){
      task.callback();
      if(task.repeating) task.remainingTime=task.interval+task.remainingTime;
      else taskPool.remove(task.id);
    }
  }
}

