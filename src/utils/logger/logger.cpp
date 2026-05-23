#include "./logger.hpp"
#include <format>
#include <thread>


std::array<Log, MaxStoredLogs> Logger::logBuffer;


void Logger::writeLogs(){
  bufferLock.lock();
  while(readIndex!=writeIndex){
    auto& log = logBuffer[readIndex];
    auto time_t = std::chrono::system_clock::to_time_t(log.timestamp);
    std::string time_str = std::ctime(&time_t);
    time_str.pop_back();
    std::string log_message = std::format("[{}][{}][{}]:{}\n", 
        time_str, log.level, log.file, log.message);
    std::cout << log_message;
    if(file.is_open()) file << log_message;
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
    std::this_thread::sleep_for(std::chrono::seconds(1));
    writeLogs();
    flush();
  }
}
