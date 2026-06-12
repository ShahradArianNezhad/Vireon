#include "GLFWwindow.hpp"
#include "engine/graphics/renderer/renderer.hpp"
#include <GLFW/glfw3.h>
#include "stb_image.h"

void EngineWindow::initGLFW() {
  if (!glfwInit()) {
    LOG_FATAL("Failed to initialize GLFW");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  LOG_INFO("initializing GLFW");
}

void EngineWindow::createWindow() {
  window = glfwCreateWindow(Screen::width, Screen::height, window_name.c_str(), nullptr, nullptr);
  checkWindowInit();
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);
  Renderer::initGLAD();
  glViewport(0, 0,Screen::width, Screen::height);
  glfwSetFramebufferSizeCallback(window, sizeChange_callback);
  LOG_INFO("window created width : {} height : {}",Screen::width,Screen::height);
}


void EngineWindow::setDisplayCursor(Cursor mode){
  switch(mode){
    case HIDDEN:
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
      break;
    case NORMAL:
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      break;
    case DISABLED:
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      break;
  }
}


void EngineWindow::checkWindowInit() {
  if (window == NULL) {
    LOG_FATAL("Failed to initialize window");
  }
  LOG_INFO("window initialized");
}

EngineWindow::EngineWindow(uint32_t w, uint32_t h,std::string name){
  window_name=name;
  Screen::width=w;
  Screen::height=h;
  initGLFW();
  createWindow();
};

void EngineWindow::setWindowSize(int w,int h){
  glfwSetWindowSize(window, w, h);
}


void EngineWindow::setWindowName(std::string name){
  glfwSetWindowTitle(window, name.c_str());
}
void EngineWindow::setWindowIcon(std::string iconPath){
    int w,h,nrChannels;
    unsigned char* data = stbi_load(iconPath.c_str(), &w, &h,&nrChannels, 4);
    if(!data){
      LOG_ERROR("couldnt load icon={}",iconPath);
      return;
    }
    GLFWimage image;
    image.width=w;
    image.height=h;
    image.pixels=data;
    glfwSetWindowIcon(window, 1, &image);
    LOG_INFO("window icon set : {}",iconPath);
    stbi_image_free(data);
}

void EngineWindow::setFullscreen(size_t monitor){
  int count;
  GLFWmonitor** monitors = glfwGetMonitors(&count);
  const GLFWvidmode* mode = glfwGetVideoMode(monitors[monitor]);
  if(monitor>(size_t)count){
    LOG_FATAL("setFullscreen called on invalid monitor ={}",monitor);
    return;
  }
 
  glfwSetWindowMonitor(window, monitors[monitor], 0, 0, mode->width, mode->height, mode->refreshRate);
  Screen::width = mode->width;
  Screen::height = mode->height;
  LOG_INFO("window set fullscreen on monitor : {}",monitor);
}

void EngineWindow::setWindowed(){
  int w=800;
  int h=600;
  glfwSetWindowMonitor(window, NULL, 0, 0, w ,h ,0);
  LOG_INFO("window set windowed");
}


EngineWindow::~EngineWindow() { cleanup(); }

bool EngineWindow::windowShouldClose() { return glfwWindowShouldClose(window); }

void EngineWindow::updateWindow() {
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void EngineWindow::cleanup() {
  LOG_INFO("destroying window");
  glfwDestroyWindow(window);
  glfwTerminate();
}
