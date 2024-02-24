#pragma once
#include <cmath>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

#include "imgui.h"

inline ImVec2 operator+(const ImVec2 &a, const ImVec2 &b) {
  return ImVec2(a.x + b.x, a.y + b.y);
}
inline ImVec2 operator-(const ImVec2 &a, const ImVec2 &b) {
  return ImVec2(a.x - b.x, a.y - b.y);
}
inline ImVec2 &operator+=(ImVec2 &a, const ImVec2 &b) {
  a = a + b;
  return a;
}
inline ImVec2 &operator-=(ImVec2 &a, const ImVec2 &b) {
  a = a - b;
  return a;
}

inline std::ostream &operator<<(std::ostream &os, const ImVec2 &point) {
  os << "ImVec2{x=" << point.x << ", y=" << point.y << "}";
  return os;
}

class NodeEditor {
  static constexpr float padding = 6;
  static constexpr float mark = 6;

  struct Node {
    ImVec2 pos;
    std::string title;
  };

  std::vector<Node> nodes;

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
        nodes.emplace_back(Node{ImGui::GetMousePos(), "NewNode"});
      }
      ImGui::EndPopup();
    }
  }

  void DrawNodes() {
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    auto white_col = IM_COL32(255, 255, 255, 255);

    for (auto &node : nodes) {
      auto &title = node.title;
      auto size =
          ImGui::CalcTextSize(title.c_str(), title.c_str() + title.size());
      auto cur_p0 = node.pos;
      auto cur_p1 = cur_p0 + size + ImVec2{padding * 2, padding * 2};
      draw_list->AddText(cur_p0 + ImVec2{padding, padding}, white_col,
                         title.c_str(), title.c_str() + title.size());
      draw_list->AddRect(cur_p0, cur_p1, white_col);

      ImGui::SetCursorPos(node.pos);
      ImGui::InvisibleButton("## node", cur_p1 - cur_p0,
                             ImGuiButtonFlags_MouseButtonLeft);
      if (ImGui::IsItemActive() &&
          ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        node.pos += io.MouseDelta;
      }
    }
  }
  void DrawConns() {}

  void DrawLoop() {
    ImGui::Begin("neditor", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

    DrawMenuBar();
    DrawPopup();

    DrawNodes();
    DrawConns();

    ImGui::End();
  }
};
