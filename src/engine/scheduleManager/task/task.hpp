#pragma once
#include <cstdint>
#include <functional>

using TaskId = uint32_t;

struct Task{
  TaskId id;
  float remainingTime;
  float interval;
  bool repeating;
  std::function<void()> callback;
};
