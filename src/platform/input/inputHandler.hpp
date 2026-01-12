#pragma once
#include <GLFW/glfw3.h>

class InputHandler {
public:
  InputHandler(GLFWwindow *window);
  bool checkKeyPress(int key);

private:
  GLFWwindow *window;
};
