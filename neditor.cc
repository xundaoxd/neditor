#include <cstdio>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GLFW/glfw3.h" // Will drag system OpenGL headers

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  if (!glfwInit()) {
    return 1;
  }
  glfwSetErrorCallback(glfw_error_callback);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  GLFWwindow *window = glfwCreateWindow(1280, 720, "neditor", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
