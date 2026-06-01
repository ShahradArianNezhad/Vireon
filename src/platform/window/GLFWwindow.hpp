#pragma once
#include "utils/logger/logger.hpp"
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>


namespace Screen {
inline uint32_t width = 800;
inline uint32_t height = 600;
} // namespace Screen

class EngineWindow {
private:
  GLFWwindow *window=nullptr;
  std::string window_name;

public:

  EngineWindow(uint32_t w, uint32_t h,std::string windowName);
  EngineWindow(const EngineWindow &) = delete;
  EngineWindow &operator=(const EngineWindow &) = delete;
  EngineWindow(EngineWindow &&) = delete;
  ~EngineWindow();
  GLFWwindow* getWindowPtr(){return window;}
  bool windowShouldClose();
  void updateWindow();
  void setViewport();
  void setWindowName(std::string name);
  void setFullscreen(size_t monitor);
  void setWindowed();
  void setWindowSize(int w,int h);
  void setWindowIcon(std::string iconPath);


private:

  void initGLFW();
  void createWindow();
  void cleanup();
  void checkWindowInit();

  static void sizeChange_callback(GLFWwindow *, int width, int height) {
    //glViewport(0, 0, width, height);
    Screen::width = width;
    Screen::height = height;
    LOG_INFO("window size changed: {}x{}",width,height);
  }
};
