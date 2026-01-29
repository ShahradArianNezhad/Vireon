#pragma once

#include <queue>
class IDManager {
private:
  unsigned int next_count = 0;
  std::queue<unsigned int> freeIds;

public:
  IDManager() {};
  unsigned int get();
  void release(unsigned int id);
};
