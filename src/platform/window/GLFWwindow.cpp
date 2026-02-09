#include "GLFWwindow.hpp"
#include "engine/graphics/renderer/renderer.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

void EngineWindow::initGLFW() {
  if (!glfwInit()) {
    throw std::runtime_error("failed to initialize GLFW");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void EngineWindow::createWindow() {
  window = glfwCreateWindow(Screen::width, Screen::height, "my engine", nullptr, nullptr);
  checkWindowInit();
  glfwMakeContextCurrent(window);
  Renderer::initGLAD();
  glViewport(0, 0,Screen::width, Screen::height);
  glfwSetFramebufferSizeCallback(window, sizeChange_callback);
}

void EngineWindow::checkWindowInit() {
  if (window == NULL) {
    throw std::runtime_error{"Failed to initialize GLFW window"};
  }
}

EngineWindow::EngineWindow(uint32_t w, uint32_t h){
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
