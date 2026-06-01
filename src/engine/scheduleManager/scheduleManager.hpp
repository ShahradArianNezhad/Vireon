#pragma once
#include "utils/idManager/idManager.hpp"
#include "utils/objectPool/objectPool.hpp"
#include "./task/task.hpp"


class ScheduleManager{
  IDManager idManager;
  ObjectPool<Task> taskPool;

  public:

  TaskId do_after(float secs,std::function<void()> callback); 
  TaskId do_every(float secs,std::function<void()> callback); 
  void cancel_task(TaskId id);
  void update(float dt);
};
