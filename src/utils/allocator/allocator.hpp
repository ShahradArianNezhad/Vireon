#pragma once
#include "utils/logger/logger.hpp"
#include "utils/types.hpp"
#include <cstddef>
#include <string>



template<typename T>
class EngineAllocator:public AllocatorBase{
  public:
    typedef T value_type;
    size_t index;

    EngineAllocator(const std::string& name){
      this->allocatorName=name;
      if(Logger::freeSlots.empty()){
        index = Logger::allocators.size();
        Logger::allocators.push_back(AllocatorBase{0,name});
      }
      else{
        index = Logger::freeSlots.back();
        Logger::freeSlots.pop_back();
        Logger::allocators[index] = AllocatorBase{0,name};
      }
    };

    ~EngineAllocator(){
      Logger::freeSlots.push_back(index);
    }

    template<class U>
      EngineAllocator(const EngineAllocator<U>& other) {
        this->allocatorName=other.allocatorName;
        this->allocatedBytes=other.allocatedBytes;
        this->index=other.index;
      }

    T* allocate(std::size_t n){
      auto& stat = Logger::allocators[index];
      stat.allocatedBytes += n*sizeof(T);
      return static_cast<T*>(::operator new(n*sizeof(T)));
    }
    void deallocate(T* p, std::size_t n) noexcept{
      auto& stat = Logger::allocators[index];
      stat.allocatedBytes -= n*sizeof(T);
      ::operator delete(p);
    }
};
