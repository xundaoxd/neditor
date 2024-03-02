#pragma once
#include <iostream>
#include <list>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include <GLFW/glfw3.h>

#include "NodeGraph.h"
#include "NodeProperty.h"

class NodeEditor {
  GLFWwindow *window;

  NodeGraph ngraph;
  NodeProperty nproperty;

  bool property_view{true};
  bool metrics_view{false};

public:
  void Init(GLFWwindow *window_, const char *glsl_version) {
    window = window_;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;
    io.IniFilename = nullptr;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();
  }
  void UpdateMenu() {
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("New Node")) {
          ngraph.NewNode();
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("View")) {
        if (ImGui::MenuItem("Property")) {
          property_view = !property_view;
        }
        if (ImGui::MenuItem("Metrics")) {
          metrics_view = !metrics_view;
        }
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }

  void UpdateDock() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(main_viewport->WorkPos);
    ImGui::SetNextWindowSize(main_viewport->WorkSize);

    ImGui::Begin("Neditor", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                     ImGuiWindowFlags_NoFocusOnAppearing);

    ImGuiID dock_id = ImGui::GetID("NeditorDock");
    if (!ImGui::DockBuilderGetNode(dock_id)) {
      ImGui::DockBuilderAddNode(dock_id,
                                ImGuiDockNodeFlags_PassthruCentralNode |
                                    ImGuiDockNodeFlags_DockSpace);
      ImGui::DockBuilderSetNodeSize(dock_id, main_viewport->WorkSize);
      ImGuiID dock_left, dock_right;
      ImGui::DockBuilderSplitNode(dock_id, ImGuiDir_Right, 0.2f, &dock_right,
                                  &dock_left);
      ImGui::DockBuilderDockWindow("NodeGraph", dock_left);
      ImGui::DockBuilderDockWindow("PropertyView", dock_right);
      ImGui::DockBuilderFinish(dock_id);
    }
    ImGui::DockSpace(dock_id, main_viewport->WorkSize,
                     ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::End();

    ImGui::PopStyleVar(3);
  }

  void Update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    UpdateMenu();
    UpdateDock();

    ngraph.Update("NodeGraph");
    if (property_view) {
      nproperty.Update("PropertyView");
    }
    if (metrics_view) {
      ImGui::ShowMetricsWindow();
    }
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
