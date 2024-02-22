#include <cstdio>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GLFW/glfw3.h" // Will drag system OpenGL headers

#include "neditor/ImNodes.h"
#include "neditor/ImNodesEz.h"

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void DoDraw() {
  static ImNodes::Ez::Context *context = ImNodes::Ez::CreateContext();
  IM_UNUSED(context);

  if (ImGui::Begin("ImNodes", nullptr,
                   ImGuiWindowFlags_NoScrollbar |
                       ImGuiWindowFlags_NoScrollWithMouse)) {
    ImNodes::Ez::BeginCanvas();

    struct Node {
      ImVec2 pos{};
      bool selected{};
      ImNodes::Ez::SlotInfo inputs[1];
      ImNodes::Ez::SlotInfo outputs[1];
    };

    static Node nodes[3] = {
        {{50, 100}, false, {{"In", 1}}, {{"Out", 1}}},
        {{250, 50}, false, {{"In", 1}}, {{"Out", 1}}},
        {{250, 100}, false, {{"In", 1}}, {{"Out", 1}}},
    };

    for (Node &node : nodes) {
      if (ImNodes::Ez::BeginNode(&node, "Node Title", &node.pos,
                                 &node.selected)) {
        ImNodes::Ez::InputSlots(node.inputs, 1);
        ImNodes::Ez::OutputSlots(node.outputs, 1);
        ImNodes::Ez::EndNode();
      }
    }

    ImNodes::Connection(&nodes[1], "In", &nodes[0], "Out");
    ImNodes::Connection(&nodes[2], "In", &nodes[0], "Out");

    ImNodes::Ez::EndCanvas();
  }
  ImGui::End();
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

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard |
                    ImGuiConfigFlags_NavEnableGamepad |
                    ImGuiConfigFlags_DockingEnable;
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  while (!glfwWindowShouldClose(window)) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    ImGui::SetNextWindowSize(
        ImVec2(width, height)); // ensures ImGui fits the GLFW window

    DoDraw();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
