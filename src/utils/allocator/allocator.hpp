#pragma once
#include "utils/logger/logger.hpp"
#include "utils/types.hpp"
#include <cstddef>
#include <string>



template<typename T>
class EngineAllocator:public AllocatorBase{
  public:
    typedef T value_type;

    EngineAllocator(const std::string& name){
      this->allocatorName=name;
      Logger::allocators.push_back(this);
    };

    EngineAllocator(){
      this->allocatorName=typeid(T).name();
      Logger::allocators.push_back(this);
    };

    ~EngineAllocator(){
      for(size_t i{};i<Logger::allocators.size();i++)
        if(Logger::allocators[i]==this){
          Logger::allocators.erase(Logger::allocators.begin() + i);
          break;
        }
    }

    template<class U>
      EngineAllocator(const EngineAllocator<U>& other) {
        this->allocatorName=other.allocatorName;
        this->allocatedBytes=other.allocatedBytes;
        for(size_t i{};i<Logger::allocators.size();i++)
          if(Logger::allocators[i]==&other)Logger::allocators[i]=this;
      }

    T* allocate(std::size_t n){
      allocatedBytes += n*sizeof(T);
      return static_cast<T*>(::operator new(n*sizeof(T)));
    }
    void deallocate(T* p, std::size_t n) noexcept{
      allocatedBytes -= n*sizeof(T);
      ::operator delete(p);
    }
};
