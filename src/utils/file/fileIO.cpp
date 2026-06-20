#include "utils/logger/logger.hpp"
#include <filesystem>
#include <utils/file/fileIO.hpp>

std::string FileIO::readText(const std::string &filePath) {
  LOG_INFO("reading file:{}",filePath);

  std::ifstream file(filePath);
  if (!file.is_open()) {
    std::cout << filePath << std::endl;
    throw std::runtime_error("Failed to read file");
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}



void FileIO::findBaseDir(){
  return;
  for(const auto& entry: std::filesystem::directory_iterator(".")){
    if(entry.is_directory() && entry.path().filename() == "vireon"){
      baseDir = entry.path();
      break;
    }
  }
  LOG_FATAL("Base directory for vireon not found, make sure the executable is in the same directory as the vireon folder");
}
