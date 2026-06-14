#include "./scheduleManager.hpp"
#include "utils/logger/logger.hpp"
#include "utils/objectPool/objectPool.hpp"


TaskId ScheduleManager::do_after(float secs,std::function<void()> callback){
  auto id = idManager.get();
  Task task{.id=id,.remainingTime=secs,.interval=secs,.repeating=false,.callback=callback};
  pendingAdds.push_back(task);
  //taskPool.add(task);
  LOG_DEBUG("do after:{} taskId:{}",secs,id);
  return id;
} 

TaskId ScheduleManager::do_every(float secs,std::function<void()> callback){
  auto id = idManager.get();
  Task task{.id=id,.remainingTime=secs,.interval=secs,.repeating=true,.callback=callback};
  pendingAdds.push_back(task);
  //taskPool.add(task);
  LOG_DEBUG("do every:{} taskId:{}",secs,id);
  return id;
} 

void ScheduleManager::cancel_task(TaskId id){
  pendingRemoves.push_back(id);
  LOG_DEBUG("task removed taskId:{}",id);
}

  void ScheduleManager::flushRemoves(){
    for(auto id:pendingRemoves){
#ifdef DEBUG_VERBOSE
      LOG_DEBUG("REMOVING : {}",id);
#endif
      taskPool.remove(id);
    }
    pendingRemoves.clear();
  }
  void ScheduleManager::flushAdds(){
    for(auto Task:pendingAdds){taskPool.add(Task);}
    pendingAdds.clear();
  }


void ScheduleManager::update(float dt){
  for(auto& task:taskPool.pool){
    task.remainingTime-=dt;
    if(task.remainingTime<=0.0f){
      task.callback();
      if(task.repeating) task.remainingTime=task.interval+task.remainingTime;
      else pendingRemoves.push_back(task.id);
    }
  }
  flushRemoves();
  flushAdds();
}

