#include "GLFWwindow.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

void EngineWindow::initGLFW() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void EngineWindow::createWindow() {
  window = glfwCreateWindow(WIDTH, HEIGHT, "my engine", nullptr, nullptr);
  if (window == NULL) {
    throw std::runtime_error{"Failed to initialize GLFW window"};
  }
  glfwMakeContextCurrent(window);
  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(window, sizeChange_callback);
}

EngineWindow::EngineWindow(uint32_t w, uint32_t h) : WIDTH(w), HEIGHT(h) {
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
