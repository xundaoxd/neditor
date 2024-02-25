#pragma once
#include <list>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include "Node.h"
#include "common.h"

class NodeEditor {
  GLFWwindow *window;

  std::list<Node> nodes;

  bool property_view{false};
  Node *current_node{nullptr};

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
          const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
          nodes.emplace_back(main_viewport->WorkPos);
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("View")) {
        if (ImGui::MenuItem("Property")) {
          property_view = !property_view;
        }
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }
  void UpdatePropertyView() {
    if (property_view) {
      ImGui::Begin("PropertyView");
      // TODO: impl
      if (current_node) {
        ImGui::TextUnformatted(current_node->title.c_str(),
                               current_node->title.c_str() +
                                   current_node->title.size());
      }
      ImGui::End();
    }
  }
  void UpdateConns() {
    for (auto &node : nodes) {
      node.UpdateLinks();
    }
  }
  void UpdateNodes() {
    for (auto &node : nodes) {
      node.UpdateNode();
      if (node.IsSelected()) {
        current_node = &node;
      }
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
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("neditor", nullptr, window_flags);
    UpdateMenu();
    UpdatePropertyView();
    UpdateConns();
    UpdateNodes();
    Node::FinishUpdate();
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
