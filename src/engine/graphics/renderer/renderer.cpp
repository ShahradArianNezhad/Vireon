#include "renderer.hpp"
#include "engine/graphics/batch/batch.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glad/gl.h>
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
  if (batch.isUsingEbo()) {
    glDrawElements(GL_TRIANGLES, batch.getIndicesCount(), GL_UNSIGNED_INT,
                   (void *)0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, batch.getIndicesCount());
  }
}

void Renderer::mainloop() {
  while (!window.windowShouldClose()) {
    clear();
    render();
    window.updateWindow();
  }
}

void Renderer::addObject(Object object) {
  for (Batch &batch : batches) {
    if (batch.layout.getLayoutArray() == object.layout.getLayoutArray()) {
      batch.submit(object);
      return;
    }
  }
  Shader newshader("./shaders/simple.vert", "./shaders/simple.frag");
  Batch newBatch(std::move(newshader), object.layout);
  newBatch.submit(object);
  batches.push_back(std::move(newBatch));
}
