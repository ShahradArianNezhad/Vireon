#pragma once
#include <string>

class FileIO {
public:
  FileIO() = delete;
  static std::string readText(const std::string &filePath);
};
