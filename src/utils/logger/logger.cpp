#include "./logger.hpp"
#include <format>
#include <mutex>
#include <thread>
#include <filesystem>
#include <string>


std::array<Log, MaxStoredLogs> Logger::logBuffer;


std::string logLevelToColor(LogLevel level){
  switch (level){
  case Debug: return "\033[36m";      // Cyan
  case Info:  return "\033[32m";      // Green
  case Warn:  return "\033[38;5;208m";      // Yellow
  case Error: return "\033[91m";      // Red
  case Fatal: return "\033[97;101m";    // Bright + Red Background
  }
  return "unkown";
}

std::string get_file_stem(std::string path) {
    return std::filesystem::path(path).stem().string();
}

void Logger::writeLogs(){
  std::string endColor="\033[0m";
  bufferLock.lock();
  while(readIndex!=writeIndex){
    auto& log = logBuffer[readIndex];
    auto time_t = std::chrono::system_clock::to_time_t(log.timestamp);
    std::tm tm = *std::localtime(&time_t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");

    std::string time_str = oss.str();
    std::string console_log_message = std::format("{}[{}][{}][{}]{}:{}\n", 
        logLevelToColor(log.level),time_str, log.level, get_file_stem(log.file),endColor, log.message);
    std::string file_log_message = std::format("[{}][{}][{}]:{}\n", 
        time_str, log.level, get_file_stem(log.file), log.message);
    std::cout << console_log_message;
    if(file.is_open()) file << file_log_message;
    readIndex = (readIndex+1)%MaxStoredLogs;
  }
  bufferedLogs=0;
  bufferLock.unlock();
}


void Logger::bufferLog(Log log){
  bufferLock.lock();
  logBuffer[writeIndex] = log;
  bufferedLogs+=1;
  writeIndex = (writeIndex+1)%MaxStoredLogs;
  bufferLock.unlock();

  if(bufferedLogs>=MaxStoredLogs/2){
    writeLogs();
    flush();
  }
}



void Logger::workerFunction(){
  while(running){
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    std::unique_lock<std::mutex> lock{workerMutex};
    cv.wait_for(lock,std::chrono::seconds(1));
    writeLogs();
    flush();
  }
}
