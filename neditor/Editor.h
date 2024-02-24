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
  static constexpr float rounding = 4;

  struct Node {
    ImVec2 pos;
    std::string title;

    std::vector<std::string> islots;
    std::vector<std::string> oslots;

    Node(ImVec2 pos, std::string title, std::vector<std::string> islots = {},
         std::vector<std::string> oslots = {})
        : pos(std::move(pos)), title(std::move(title)),
          islots(std::move(islots)), oslots(std::move(oslots)) {}
  };

  std::vector<Node> nodes{
      {{100, 100}, "demo", {"in0", "in1"}, {"out0", "out1", "out2"}},
      {{200, 200}, "demo", {"in0", "in1"}, {"out0", "out1", "out2"}}};

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
        nodes.emplace_back(ImGui::GetMousePos(), "NewNode");
        // TODO: impl
      }
      ImGui::EndPopup();
    }
  }

  void DrawNode(Node *node) {
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    ImU32 col = IM_COL32(255, 255, 255, 255);

    auto node_p0 = node->pos;
    auto cur_pos = node_p0;

    auto header_p0 = cur_pos;
    // draw title
    auto title_sz = ImGui::CalcTextSize(
        node->title.c_str(), node->title.c_str() + node->title.size());
    auto title_p0 = cur_pos + ImVec2{padding, padding};
    draw_list->AddText(title_p0, col, node->title.c_str(),
                       node->title.c_str() + node->title.size());
    cur_pos += ImVec2{0, padding * 2 + title_sz.y};
    // draw title end

    // draw slots
    auto slots_p0 = cur_pos;
    // draw in slots
    auto islots_p0 = cur_pos + ImVec2{padding, 0};
    auto islot_p0 = islots_p0;
    float islots_width = title_sz.x;
    ImVec2 radio_sz;
    for (auto &slot : node->islots) {
      islot_p0 += ImVec2{0, padding};
      ImGui::SetCursorPos(islot_p0);
      ImGui::PushID(&slot);
      ImGui::RadioButton("##", false);
      if (ImGui::IsItemActive() &&
          ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        std::cout << "radio released" << std::endl;
        // TODO: impl
      }
      ImGui::PopID();
      radio_sz = ImGui::GetItemRectSize();
      auto text_sz =
          ImGui::CalcTextSize(slot.c_str(), slot.c_str() + slot.size());
      islots_width = std::max(islots_width, radio_sz.x + text_sz.x);
      draw_list->AddText(islot_p0 + ImVec2{radio_sz.x, 0}, col, slot.c_str(),
                         slot.c_str() + slot.size());
      islot_p0 += ImVec2{0, radio_sz.y};
    }
    auto islots_p1 = islot_p0 + ImVec2{islots_width, padding};
    draw_list->AddRect(islots_p0, islots_p1, col);
    // draw in slots end
    // draw out slots
    float oslots_width = 0;
    for (auto &slot : node->oslots) {
      auto text_sz =
          ImGui::CalcTextSize(slot.c_str(), slot.c_str() + slot.size());
      oslots_width = std::max(oslots_width, radio_sz.x + text_sz.x);
    }
    auto oslots_p0 =
        islots_p0 + ImVec2{islots_width + padding * 2 + oslots_width, 0};
    auto oslot_p0 = oslots_p0;
    for (auto &slot : node->oslots) {
      oslot_p0 += ImVec2{0, padding};
      auto radio_p0 = oslot_p0 - ImVec2{radio_sz.x, 0};
      ImGui::SetCursorPos(radio_p0);
      ImGui::PushID(&slot);
      ImGui::RadioButton("##", false);
      if (ImGui::IsItemActive() &&
          ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        std::cout << "radio clicked" << std::endl;
        // TODO: impl
      }
      ImGui::PopID();
      auto text_sz =
          ImGui::CalcTextSize(slot.c_str(), slot.c_str() + slot.size());
      auto text_p0 = radio_p0 - ImVec2{text_sz.x, 0};
      draw_list->AddText(text_p0, col, slot.c_str(),
                         slot.c_str() + slot.size());
      oslot_p0 += ImVec2{0, radio_sz.y};
    }
    auto oslots_p1 = oslot_p0 + ImVec2{0, padding};
    draw_list->AddRect(oslots_p0 - ImVec2{oslots_width, 0}, oslots_p1, col);
    // draw out slots end
    auto slots_p1 = oslots_p1 + ImVec2{padding, 0};
    draw_list->AddRect(slots_p0, slots_p1, col);
    // draw slots end
    auto node_p1 = slots_p1;
    draw_list->AddRect(node_p0, node_p1, col, rounding);

    // draw header
    auto header_p1 = oslots_p0 + ImVec2{padding, 0};
    draw_list->AddLine(ImVec2{header_p0.x, header_p1.y}, header_p1, col);
    ImGui::SetCursorPos(header_p0);
    ImGui::PushID(&node->title);
    ImGui::InvisibleButton("##", header_p1 - header_p0,
                           ImGuiButtonFlags_MouseButtonLeft);
    if (ImGui::IsItemActive() &&
        ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
      node->pos += io.MouseDelta;
    }
    ImGui::PopID();
    // draw header end
  }

  void DrawNodes() {
    for (auto &node : nodes) {
      DrawNode(&node);
    }
  }
  void DrawConns() {}

  void DrawLoop() {
    ImGui::Begin("neditor", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

    DrawMenuBar();
    DrawPopup();

    DrawNodes();
    // DrawConns();

    ImGui::End();
  }
};
