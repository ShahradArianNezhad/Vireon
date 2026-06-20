#include "utils/logger/logger.hpp"
#include <filesystem>
#include <utils/file/fileIO.hpp>

std::string FileIO::readText(const std::string &filePath) {
  LOG_INFO("reading file:{}",(baseDir/filePath).string());

  std::ifstream file(baseDir/filePath);
  if (!file.is_open()) {
    LOG_FATAL("failed to read file {}",(baseDir/filePath).string());
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}



void FileIO::findBaseDir(){
  for(const auto& entry: std::filesystem::recursive_directory_iterator(".")){
    if(entry.is_directory() && entry.path().filename() == "vireon"){
      for(const auto& subEntry: std::filesystem::directory_iterator(entry)){
        if(subEntry.path().filename()=="shaders"){
          baseDir = entry.path();
          return;
        }
      }
    }
  }
  for(const auto& entry: std::filesystem::recursive_directory_iterator("../")){
    if(entry.is_directory() && entry.path().filename() == "vireon"){
      for(const auto& subEntry: std::filesystem::directory_iterator(entry)){
        if(subEntry.path().filename()=="shaders"){
          baseDir = entry.path();
          return;
        }
      }
    }
  }
  LOG_FATAL("Base directory for vireon not found, make sure the executable is in the same directory as the vireon folder");
}
