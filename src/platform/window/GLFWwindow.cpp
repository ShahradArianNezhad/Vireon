#include "GLFWwindow.hpp"
#include "engine/graphics/renderer/renderer.hpp"
#include <GLFW/glfw3.h>

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

EngineWindow::~EngineWindow() { cleanup(); }

bool EngineWindow::windowShouldClose() { return glfwWindowShouldClose(window); }

void EngineWindow::updateWindow() {
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void EngineWindow::cleanup() {
  glfwDestroyWindow(window);
  glfwTerminate();
}
