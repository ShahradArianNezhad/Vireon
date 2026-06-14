#pragma once
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <utility>
#include "./utils/types.hpp"
//#define ENGINE_DEBUG

constexpr int MaxStoredLogs = 200;

enum LogLevel{
  Debug,
  Info,
  Warn,
  Error,
  Fatal
};


constexpr std::string_view to_string(LogLevel c) {
  switch (c) {
    case LogLevel::Debug:   return "DEBUG";
    case LogLevel::Info: return "INFO";
    case LogLevel::Warn:  return "WARN";
    case LogLevel::Error:  return "ERROR";
    case LogLevel::Fatal:  return "FATAL";
  }
  return "Unknown";
}

template <>
struct std::formatter<LogLevel> : std::formatter<std::string_view> {
  auto format(LogLevel c, format_context& ctx) const {
    return std::formatter<std::string_view>::format(
        to_string(c),
        ctx
        );
  }
};



#ifdef ENGINE_DEBUG
#define LOG_DEBUG(...)\
  Logger::log(LogLevel::Debug,__FILE__,__VA_ARGS__)
#else
#define LOG_DEBUG(...) 
#endif

#define LOG_INFO(...)\
  Logger::log(LogLevel::Info,__FILE__,__VA_ARGS__)

#define LOG_WARN(...)\
  Logger::log(LogLevel::Warn,__FILE__,__VA_ARGS__)

#define LOG_ERROR(...)\
  Logger::log(LogLevel::Error,__FILE__,__VA_ARGS__)

#define LOG_FATAL(...)\
  Logger::log(LogLevel::Fatal,__FILE__,__VA_ARGS__)

#define LOG_MEMORY()\
  for(size_t i{};i<Logger::allocators.size();i++){\
    AllocatorStats alloc = Logger::allocators[i];\
    LOG_INFO("allocator name : {}, bytes allocated : {}B",alloc.allocatorName,alloc.allocatedBytes);\
  }\


struct Log{
  std::chrono::system_clock::time_point timestamp;
  LogLevel level;
  std::string file;
  std::string message;
};

class Logger{
  private:
  static std::array<Log, MaxStoredLogs> logBuffer;
  static inline LogLevel logLevel = LogLevel::Info;
  static inline std::atomic<bool> running = false;
  static inline std::ofstream file;
  static inline std::string logFilePath;
  static inline int readIndex = 0;
  static inline int writeIndex = 0;
  static inline size_t bufferedLogs=0;
  static inline std::mutex bufferLock;
  static inline std::mutex workerMutex;
  static inline std::thread logger_thread;
  static inline std::condition_variable cv;
  static void workerFunction();

  static void writeLogs();
  static inline void flush(){
    std::cout.flush();
    file.flush();
  }
  static void bufferLog(Log log);

  public:
  static inline std::vector<AllocatorStats> allocators;
  static inline std::vector<size_t> freeSlots;

  ~Logger(){
    writeLogs();
    flush();
    stopLogger();
  }


  static void startLogger(const std::string& logFile=""){
    running=true;
    logger_thread = std::thread(workerFunction);
    if(logFile!=""){
      file.open(logFile);
      logFilePath=logFile;
    }
    if(logFilePath!="")LOG_INFO("logging with log file:{}",logFilePath);
    else LOG_INFO("logging with no log file");
  }
  static void stopLogger(){
    if(!running)return;
    LOG_INFO("logger stopping...");
    running=false;
    cv.notify_one();
    logger_thread.join();
  }
  static inline void setLogLevel(LogLevel level){
    logLevel=level;
    LOG_INFO("logLevel set:{}",level);
  }
  static inline LogLevel getLogLevel(){return logLevel;}

  template<typename... Args>
  static inline void log(LogLevel level,std::string filePath,std::format_string<Args...> fmt,Args&&... args){
    if(level<logLevel) return;
    bufferLog(Log{std::chrono::system_clock::now(),level,filePath,std::format(fmt,std::forward<Args>(args)...)});
    if(level==LogLevel::Fatal){
      writeLogs();
      flush();
      stopLogger();
    }
  }




};


