#include <unistd.h>
#define MINIAUDIO_IMPLEMENTATION
#include "./audioManager.hpp"
#include "utils/logger/logger.hpp"




void AudioManager::init(){
 ma_result result;
  result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS) LOG_FATAL("Audio engine initialization failed");
}


void AudioManager::uninit(){
  ma_engine_uninit(&engine);
}


AudioManager::Sound AudioManager::playSound(const std::string& path,bool looped){
  auto id = idmanager.get();
  Sound sound{id};
  playing.add(sound);
  auto* addr=&playing.get(id).ma_soundPtr;

  auto it = soundCache.find(path);
  if(it!=soundCache.end()){
    ma_sound_init_copy(
        &engine,
        &(it->second),
        MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM,
        NULL,
        addr);
  }else{
    ma_sound_init_from_file(
        &engine,
        path.c_str(),
        MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM,
        NULL,
        NULL,
        addr);
  }
  LOG_DEBUG("playing audio : {}",path);
  ma_sound_start(addr);
  //ma_sound_set_end_callback(addr,soundEndedCallback , NULL);
  ma_sound_set_looping(addr, looped);
  return id;
}

void AudioManager::cacheSound(const std::string& path){
  auto it = soundCache.find(path);
  if(it!=soundCache.end()){
    LOG_WARN("trying to cache a pre cached audio : {}",path);
    return;
  }
  LOG_INFO("caching audio file : {}",path);
  ma_sound_init_from_file(
      &engine,
      path.c_str(),
      MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM,
      NULL,
      NULL,
      &soundCache[path]);
}

void AudioManager::stopSound(Sound& sound){
  auto* soundptr = &playing.get(sound.id).ma_soundPtr;
  ma_sound_stop(soundptr);
}

void AudioManager::stopSoundWithFadeMs(Sound& sound,unsigned int ms){
  auto* soundptr = &playing.get(sound.id).ma_soundPtr;
  ma_sound_stop_with_fade_in_milliseconds(soundptr, ms);
}

void AudioManager::deleteSound(Sound& sound){
  auto* soundptr = &playing.get(sound.id);
  if(!ma_sound_at_end(&soundptr->ma_soundPtr)){
    ma_sound_set_end_callback(&soundptr->ma_soundPtr, endCallback, soundptr);
    return;
  }
  ma_sound_uninit(&soundptr->ma_soundPtr);
  playing.remove(sound.id);
  idmanager.release(sound.id);
}

void AudioManager::endCallback(void* sound,ma_sound* soundptr){
  ma_sound_uninit(soundptr);
  playing.remove((*(Sound*)sound).id);
  idmanager.release((*(Sound*)sound).id);
}
