#pragma once
#include "engine/eventManager/eventManager.hpp"
#include "utils/logger/logger.hpp"
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

struct WindowSizeChangeEvent{
  int w,h;
};

namespace Screen {
inline uint32_t width = 800;
inline uint32_t height = 600;
} // namespace Screen


enum Cursor  {
  NORMAL=GLFW_CURSOR_NORMAL,
  HIDDEN=GLFW_CURSOR_HIDDEN,
  DISABLED=GLFW_CURSOR_DISABLED
};

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
  GLFWwindow* getWindowPtr();
  bool windowShouldClose();
  void updateWindow();
  void setViewport();
  void setWindowName(std::string name);
  void setFullscreen(size_t monitor=0);
  void setWindowed();
  void setWindowSize(int w,int h);
  void setWindowIcon(std::string iconPath);
  void setDisplayCursor(Cursor mode);


private:

  void initGLFW();
  void createWindow();
  void cleanup();
  void checkWindowInit();

  static void sizeChange_callback(GLFWwindow *, int width, int height) {
    //glViewport(0, 0, width, height);
    Screen::width = width;
    Screen::height = height;
    EventManager::emit(WindowSizeChangeEvent{width,height});
    LOG_INFO("window size changed: {}x{}",width,height);
  }
};

