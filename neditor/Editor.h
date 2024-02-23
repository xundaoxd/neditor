#pragma once
#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"

inline ImVec2 operator+(const ImVec2 &a, const ImVec2 &b) {
  return ImVec2(a.x + b.x, a.y + b.y);
}
inline ImVec2 operator-(const ImVec2 &a, const ImVec2 &b) {
  return ImVec2(a.x - b.x, a.y - b.y);
}

inline std::ostream &operator<<(std::ostream &os, const ImVec2 &point) {
  os << "ImVec2{x=" << point.x << ", y=" << point.y << "}";
  return os;
}

class NodeEditor {
public:
  void DrawMenuBar() {
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open")) {
          // TODO:
          std::cout << "Open" << std::endl;
        }
        if (ImGui::MenuItem("Save")) {
          // TODO:
          std::cout << "Save" << std::endl;
        }
        if (ImGui::MenuItem("Close")) {
          // TODO:
          std::cout << "Close" << std::endl;
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
  }
  void DrawPopup() {
    if (ImGui::BeginPopupContextWindow()) {
      if (ImGui::MenuItem("New Node")) {
        // TODO:
        std::cout << "New Node" << std::endl;
      }
      ImGui::EndPopup();
    }
  }

  void DrawLoop() {
    ImGui::Begin("neditor", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

    DrawMenuBar();
    DrawPopup();

    ImGui::End();
  }
};
