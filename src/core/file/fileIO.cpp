#include <core/file/fileIO.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

std::string FileIO::readText(const std::string &filePath) {

  std::ifstream file(filePath);
  if (!file.is_open()) {
    std::cout << filePath << std::endl;
    throw std::runtime_error("Failed to read file");
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
