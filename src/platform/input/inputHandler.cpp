#include "inputHandler.hpp"
#include <GLFW/glfw3.h>

InputHandler::InputHandler(GLFWwindow *window) : window(window) {}
bool InputHandler::checkKeyPress(int key) {
  return glfwGetKey(window, key) == GLFW_PRESS;
}
