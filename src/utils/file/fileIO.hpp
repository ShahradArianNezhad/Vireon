#pragma once
#include <filesystem>
#include <string>

class FileIO {
public:
  FileIO() = delete;
  static void findBaseDir();
  static std::string readText(const std::string &filePath);
private:
  static inline std::filesystem::path baseDir;
};
