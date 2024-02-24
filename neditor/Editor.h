#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <numeric>
#include <utility>
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

    std::vector<ImVec2> islots_pos;
    std::vector<ImVec2> oslots_pos;

    void AppendIslot(std::string slot) {
      islots.emplace_back(std::move(slot));
      islots_pos.emplace_back(0, 0);
    }
    void AppendOslot(std::string slot) {
      oslots.emplace_back(std::move(slot));
      oslots_pos.emplace_back(0, 0);
    }

    Node(ImVec2 pos, std::string title, std::vector<std::string> islots = {},
         std::vector<std::string> oslots = {})
        : pos(std::move(pos)), title(std::move(title)),
          islots(std::move(islots)), oslots(std::move(oslots)),
          islots_pos(this->islots.size(), ImVec2{0, 0}),
          oslots_pos(this->oslots.size(), ImVec2{0, 0}) {}
  };

  std::vector<Node> nodes{
      {{100, 100}, "demo", {"in0", "in1"}, {"out0", "out1", "out2"}},
      {{200, 200}, "demo", {"in0", "in1"}, {"out0", "out1", "out2"}}};

  std::vector<
      std::pair<std::pair<Node *, std::size_t>, std::pair<Node *, std::size_t>>>
      conns;

  bool in_linking{false};
  std::pair<Node *, std::size_t> link_src;

  void Unlink(Node *node, std::size_t idx) {
    conns.erase(std::remove_if(conns.begin(), conns.end(),
                               [=](auto &item) {
                                 return item.second ==
                                        std::make_pair(node, idx);
                               }),
                conns.end());
  }

  bool in_editing{false};
  Node *node_editing;
  int target_editing; // 0 for title, 1 for islot, 2 for oslot
  std::size_t slot_editing;
  char editing_buf[64];

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

  void DrawBezierCubic(ImVec2 p1, ImVec2 p4,
                       ImU32 col = IM_COL32(255, 255, 255, 255),
                       float thickness = 1) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    auto p2 = p1 + ImVec2{(p4.x - p1.x) * 0.6f, 0};
    auto p3 = p4 - ImVec2{(p4.x - p1.x) * 0.6f, 0};
    draw_list->AddBezierCubic(p1, p2, p3, p4, col, thickness);
  }

  void DrawNode(Node *node, ImU32 col = IM_COL32(255, 255, 255, 255)) {
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    auto node_p0 = node->pos;
    auto cur_pos = node_p0;

    auto header_p0 = cur_pos;
    // draw title
    auto title_sz = ImGui::CalcTextSize(
        node->title.c_str(), node->title.c_str() + node->title.size());
    auto title_p0 = cur_pos + ImVec2{padding, padding};
    if (in_editing && node_editing == node && target_editing == 0) {
      ImGui::SetCursorPos(title_p0);
      ImGui::PushID(node);
      if (ImGui::InputText("##", editing_buf, sizeof(editing_buf),
                           ImGuiInputTextFlags_EnterReturnsTrue)) {
        in_editing = false;
        node->title = editing_buf;
      }
      ImGui::PopID();
    } else {
      draw_list->AddText(title_p0, col, node->title.c_str(),
                         node->title.c_str() + node->title.size());
      ImGui::SetCursorPos(title_p0);
      ImGui::InvisibleButton("##", title_sz, ImGuiButtonFlags_MouseButtonLeft);
      if (ImGui::IsItemHovered() &&
          ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
        strncpy(editing_buf, node->title.c_str(), sizeof(editing_buf));
        in_editing = true;
        node_editing = node;
        target_editing = 0;
      }
    }
    cur_pos += ImVec2{0, padding * 2 + title_sz.y};
    // draw title end

    // draw slots
    auto slots_p0 = cur_pos + ImVec2{padding, padding};
    // draw in slots
    auto islots_p0 = slots_p0;
    auto islot_p0 = islots_p0;
    float islots_width = title_sz.x;
    ImVec2 radio_sz;
    for (std::size_t i = 0; i < node->islots.size(); i++) {
      auto &slot = node->islots[i];
      ImGui::SetCursorPos(islot_p0);
      ImGui::PushID(&slot);
      ImGui::RadioButton("##", false);
      ImGui::PopID();
      if (ImGui::IsItemActive() &&
          ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        Unlink(node, i);
      }
      if (ImGui::IsItemHovered() &&
          ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        if (in_linking) {
          Unlink(node, i);
          conns.emplace_back(link_src, std::make_pair(node, i));
          in_linking = false;
        }
      }
      radio_sz = ImGui::GetItemRectSize();
      node->islots_pos[i] = islot_p0 + ImVec2{radio_sz.x / 2, radio_sz.y / 2};
      auto text_sz =
          ImGui::CalcTextSize(slot.c_str(), slot.c_str() + slot.size());
      islots_width = std::max(islots_width, radio_sz.x + text_sz.x);
      if (in_editing && node_editing == node && target_editing == 1 &&
          slot_editing == i) {
        ImGui::SetCursorPos(islot_p0 + ImVec2{radio_sz.x, 0});
        ImGui::PushID(node);
        if (ImGui::InputText("##", editing_buf, sizeof(editing_buf),
                             ImGuiInputTextFlags_EnterReturnsTrue)) {
          in_editing = false;
          slot = editing_buf;
        }
        ImGui::PopID();
      } else {
        draw_list->AddText(islot_p0 + ImVec2{radio_sz.x, 0}, col, slot.c_str(),
                           slot.c_str() + slot.size());
        ImGui::SetCursorPos(islot_p0 + ImVec2{radio_sz.x, 0});
        ImGui::InvisibleButton("##", text_sz, ImGuiButtonFlags_MouseButtonLeft);
        if (ImGui::IsItemHovered() &&
            ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
          strncpy(editing_buf, slot.c_str(), sizeof(editing_buf));
          in_editing = true;
          node_editing = node;
          target_editing = 1;
          slot_editing = i;
        }
      }
      islot_p0 += ImVec2{0, radio_sz.y + padding};
    }

    ImVec2 bt_sz;
    {
      // draw new bt
      ImGui::SetCursorPos(islot_p0);
      ImGui::PushID(&node->islots);
      ImGui::Button("New");
      ImGui::PopID();
      if (ImGui::IsItemActive() &&
          ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        node->AppendIslot("Slot");
      }
      bt_sz = ImGui::GetItemRectSize();
      islots_width = std::max(islots_width, bt_sz.x);
      islot_p0 += ImVec2{0, bt_sz.y};
    }
    auto islots_p1 = islot_p0 + ImVec2{islots_width, 0};
    // draw_list->AddRect(islots_p0, islots_p1, col);
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
    for (std::size_t i = 0; i < node->oslots.size(); i++) {
      auto &slot = node->oslots[i];
      auto radio_p0 = oslot_p0 - ImVec2{radio_sz.x, 0};
      node->oslots_pos[i] = radio_p0 + ImVec2{radio_sz.x / 2, radio_sz.y / 2};
      ImGui::SetCursorPos(radio_p0);
      ImGui::PushID(&slot);
      ImGui::RadioButton("##", false);
      ImGui::PopID();
      if (ImGui::IsItemActive() &&
          ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        link_src = std::make_pair(node, i);
        in_linking = true;
      }
      auto text_sz =
          ImGui::CalcTextSize(slot.c_str(), slot.c_str() + slot.size());
      auto text_p0 = radio_p0 - ImVec2{text_sz.x, 0};

      if (in_editing && node_editing == node && target_editing == 2 &&
          slot_editing == i) {
        ImGui::SetCursorPos(text_p0);
        ImGui::PushID(node);
        if (ImGui::InputText("##", editing_buf, sizeof(editing_buf),
                             ImGuiInputTextFlags_EnterReturnsTrue)) {
          in_editing = false;
          slot = editing_buf;
        }
        ImGui::PopID();
      } else {
        draw_list->AddText(text_p0, col, slot.c_str(),
                           slot.c_str() + slot.size());
        ImGui::SetCursorPos(text_p0);
        ImGui::InvisibleButton("##", text_sz, ImGuiButtonFlags_MouseButtonLeft);
        if (ImGui::IsItemHovered() &&
            ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
          strncpy(editing_buf, slot.c_str(), sizeof(editing_buf));
          in_editing = true;
          node_editing = node;
          target_editing = 2;
          slot_editing = i;
        }
      }

      oslot_p0 += ImVec2{0, radio_sz.y + padding};
    }
    {
      // draw new
      auto bt_p0 = oslot_p0 - ImVec2{bt_sz.x, 0};
      ImGui::SetCursorPos(bt_p0);
      ImGui::PushID(&node->oslots);
      ImGui::Button("New");
      ImGui::PopID();
      if (ImGui::IsItemActive() &&
          ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        node->AppendOslot("Slot");
      }
      oslot_p0 += ImVec2{0, bt_sz.y};
    }
    auto oslots_p1 = oslot_p0;
    // draw_list->AddRect(oslots_p0 - ImVec2{oslots_width, 0}, oslots_p1, col);
    // draw out slots end
    auto slots_p1 = ImVec2{oslots_p1.x, std::max(oslots_p1.y, islots_p1.y)};
    // draw_list->AddRect(slots_p0, slots_p1, col);
    // draw slots end
    auto node_p1 = slots_p1 + ImVec2{padding, padding};

    // draw header
    auto header_p1 = oslots_p0 + ImVec2{padding, 0};
    draw_list->AddLine(ImVec2{header_p0.x, header_p1.y}, header_p1, col);
    ImGui::SetCursorPos(header_p0);
    ImGui::PushID(&node->title);
    ImGui::InvisibleButton("##", header_p1 - header_p0,
                           ImGuiButtonFlags_MouseButtonLeft);
    ImGui::PopID();
    if (ImGui::IsItemActive() &&
        ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
      node->pos += io.MouseDelta;
    }
    // draw header end

    draw_list->AddRect(node_p0, node_p1, col, rounding);
  }

  void DrawNodes() {
    ImU32 col = IM_COL32(255, 255, 255, 255);
    for (auto &node : nodes) {
      DrawNode(&node, col);
    }
    if (in_linking) {
      if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        in_linking = false;
      } else {
        DrawBezierCubic(link_src.first->oslots_pos[link_src.second],
                        ImGui::GetMousePos(), col);
      }
    }
  }
  void DrawConns() {
    for (auto &conn : conns) {
      auto &link_src = conn.first;
      auto &link_dst = conn.second;
      auto p0 = link_src.first->oslots_pos[link_src.second];
      auto p1 = link_dst.first->islots_pos[link_dst.second];
      DrawBezierCubic(p0, p1);
    }
  }

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
