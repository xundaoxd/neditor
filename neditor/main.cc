#include <cstdio>

#include "GLFW/glfw3.h"

#include "NodeEditor.h"

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    return 1;
  }

  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  GLFWwindow *window = glfwCreateWindow(1280, 720, "neditor", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  NodeEditor editor;
  editor.Init(window, glsl_version);

  while (!glfwWindowShouldClose(window)) {
    editor.Update();
    editor.Render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  editor.Shutdown();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
