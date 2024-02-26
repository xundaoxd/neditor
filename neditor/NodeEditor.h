#pragma once
#include <iostream>
#include <list>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include <GLFW/glfw3.h>

#include "Node.h"
#include "NodeProperty.h"
#include "common.h"

class NodeEditor {
  GLFWwindow *window;

  std::list<Node> nodes;
  NodeProperty nproperty;

  bool property_view{true};

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
  void UpdateConns() {
    for (auto &node : nodes) {
      node.UpdateLinks();
    }
  }
  void UpdateNodes() {
    for (auto &node : nodes) {
      node.UpdateNode();
    }
  }
  void DoDockSplit() {
    ImGuiID dockspace_id = ImGui::GetID("neditor dock");
    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr) {
      ImGui::DockBuilderAddNode(dockspace_id,
                                ImGuiDockNodeFlags_PassthruCentralNode |
                                    ImGuiDockNodeFlags_DockSpace);
      ImGui::DockBuilderSetNodeSize(dockspace_id, main_viewport->WorkSize);
      ImGuiID dock_left, dock_right;
      ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.2f,
                                  &dock_right, &dock_left);
      ImGui::DockBuilderDockWindow("node graph", dock_left);
      ImGui::DockBuilderDockWindow("PropertyView", dock_right);

      ImGui::DockBuilderFinish(dockspace_id);
    }
    ImGui::DockSpace(dockspace_id, main_viewport->WorkSize,
                     ImGuiDockNodeFlags_PassthruCentralNode);
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

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("neditor", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    DoDockSplit();

    ImGui::End();

    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&window_class);
    ImGui::Begin("node graph");
    UpdateMenu();
    UpdateConns();
    UpdateNodes();
    Node::FinishUpdate();
    ImGui::End();

    if (property_view) {
      ImGui::Begin("PropertyView");
      nproperty.Update();
      ImGui::End();
    }
    // ImGui::ShowDemoWindow();
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
