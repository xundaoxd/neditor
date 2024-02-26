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
          nodes.emplace_back(ImVec2{100, 100});
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
  void UpdateEditor() {
    ImGui::Begin("NodeGraph");
    for (auto &node : nodes) {
      node.UpdateLinks();
    }
    for (auto &node : nodes) {
      node.UpdateNode();
    }
    Node::FinishUpdate();
    ImGui::End();
  }

  void UpdateProperty() {
    ImGui::Begin("PropertyView");
    nproperty.Update();
    ImGui::End();
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
      ImGui::DockBuilderSplitNode(dock_id, ImGuiDir_Right, 0.4f, &dock_right,
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

    UpdateEditor();
    if (property_view) {
      UpdateProperty();
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
