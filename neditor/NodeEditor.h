#pragma once
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include "Node.h"
#include "common.h"

class NodeEditor {
  GLFWwindow *window;

  ImVec2 node_pos;

  std::vector<Node> nodes;

public:
  void Init(GLFWwindow *window, const char *glsl_version) {
    this->window = window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;
    io.IniFilename = nullptr;
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();
  }
  void UpdateMenu() {
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New")) {
          nodes.clear();
        }
        if (ImGui::MenuItem("Load")) {
          // TODO: impl
        }
        if (ImGui::MenuItem("Save")) {
          // TODO: impl
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("New Node")) {
          nodes.emplace_back(node_pos);
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("View")) {
        if (ImGui::MenuItem("Toggle Property")) {
          // TODO: impl
        }
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }
  void UpdateNodes() {
    for (auto &node : nodes) {
      node.Update();
    }
  }
  void Update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(main_viewport->WorkPos);
    ImGui::SetNextWindowSize(main_viewport->WorkSize);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("neditor", nullptr, window_flags);
    const ImGuiViewport *window_viewport = ImGui::GetWindowViewport();
    node_pos = window_viewport->WorkPos + main_viewport->WorkPos;
    UpdateMenu();
    UpdateNodes();
    ImGui::End();
  }
  void Render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }
  void Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
};
