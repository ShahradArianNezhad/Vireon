#pragma once
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
      auto index = idToIndex[id];
      if(index==pool.size()-1){
        pool.pop_back();
        idToIndex.erase(id);
      }else{
        pool[index] = pool.back();
        idToIndex[pool.back().id]=index;
        pool.pop_back();
      }
    }

};
