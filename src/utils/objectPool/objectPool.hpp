#pragma once
#include "utils/allocator/allocator.hpp"
#include "utils/logger/logger.hpp"
#include <cstdint>
#include <unordered_map>
#include <vector>


template<typename T>
class ObjectPool{
  private:
    std::unordered_map<uint32_t, size_t> idToIndex;
  public:
    std::vector<T> pool;

    void add(T obj){
      auto index = pool.size();
      pool.push_back(obj);
      idToIndex[obj.id]=index;
    }

    void remove(uint32_t id){
      if(!idToIndex.contains(id)){
        LOG_WARN("remove called on non existing ID");
        return;
      }
      auto index = idToIndex.at(id);

      if(index != pool.size() - 1){
        pool[index] = pool.back();
        idToIndex[pool.back().id] = index;
      }

      pool.pop_back();
      idToIndex.erase(id);
    }

    T& get(uint32_t id){
      if(!idToIndex.contains(id)){
        LOG_ERROR("get called on non existing ID");
      }
      return pool[idToIndex[id]];
    }
    

};
