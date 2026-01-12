#include "renderer.hpp"
#include "engine/graphics/batch/batch.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <iostream>
#include <stdexcept>

void Renderer::clear() {
  glClearColor(0, 1, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::render() {
  for (auto &batch : batches) {
    draw(batch);
  }
}

void Renderer::initGLAD() {
  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    throw std::runtime_error("ERROR: Failed to initialize glad");
  }
}

Renderer::Renderer() { initGLAD(); }

void Renderer::draw(Batch &batch) {
  batch.use();
  if (batch.layout.ebo) {
    glDrawElements(GL_TRIANGLES, batch.getIndexCount(), GL_UNSIGNED_INT,
                   (void *)0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, batch.getIndexCount());
  }
}

void Renderer::mainloop() {
  while (!window.windowShouldClose()) {
    clear();
    render();
    window.updateWindow();
  }
}
