#pragma once
#include <cstdint>
#include <map>
#include <string>
#include "./miniaudio/miniaudio.h"
#include "utils/allocator/allocator.hpp"
#include "utils/idManager/idManager.hpp"
#include "utils/objectPool/objectPool.hpp"

class AudioManager{
  public:
    struct Sound{
      uint64_t id;
      ma_sound ma_soundPtr{};
      Sound(uint64_t id){this->id=id;};
      ~Sound(){AudioManager::deleteSound(*this);};
    };
    static Sound playSound(const std::string& path, bool looped=false);
    static void stopSound(Sound& sound);
    static void stopSoundWithFadeMs(Sound& id,unsigned int ms);
    static void cacheSound(const std::string& path);
    static void deleteSound(Sound& sound);
    static void init();
    static void uninit();
  private:
    static inline IDManager idmanager;
    static inline void endCallback(void*,ma_sound*);
    static inline ObjectPool<Sound> playing{"AudioManager"};
    static inline std::map<std::string, ma_sound> soundCache;
    static inline ma_engine engine;
};

