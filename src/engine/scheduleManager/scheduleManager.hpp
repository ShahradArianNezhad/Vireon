#pragma once
#include "utils/idManager/idManager.hpp"
#include "utils/objectPool/objectPool.hpp"
#include "./task/task.hpp"


class ScheduleManager{
  static inline IDManager idManager;
  static inline ObjectPool<Task> taskPool{"ScheduleManager"};
  static inline std::vector<uint32_t> pendingRemoves;
  static inline std::vector<Task> pendingAdds;
  static void flushRemoves();
  static void flushAdds();

  public:

  static TaskId do_after(float secs,std::function<void()> callback); 
  static TaskId do_every(float secs,std::function<void()> callback); 
  static void cancel_task(TaskId id);
  static void update(float dt);
};
