#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "imgui.h"

#include "common.h"

class NodeImpl {
  static constexpr float padding = 6;

  ImVec2 pos;

  std::string title;
  std::vector<std::string> islots{"demo", "asd"};
  std::vector<std::string> oslots{"asd", "123"};

  std::vector<std::pair<NodeImpl *, std::size_t>> links;

  bool IsIslotLinked(std::size_t idx) { return false; }
  bool IsOslotLinked(std::size_t idx) { return false; }

  ImVec2 UpdateTitle(ImVec2 pos, ImU32 col = IM_COL32(255, 255, 255, 255)) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddText(pos, col, title.c_str(), title.c_str() + title.size());
    auto size =
        ImGui::CalcTextSize(title.c_str(), title.c_str() + title.size());
    return size;
  }

  ImVec2 UpdateIslots(ImVec2 pos, ImVec2 &radio_sz,
                      ImU32 col = IM_COL32(255, 255, 255, 255)) {
    if (islots.empty()) {
      return {0, 0};
    }

    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    auto islots_p0 = pos;

    float max_w = 0;
    ImGui::SetCursorScreenPos(pos);
    ImGui::RadioButton("##", IsIslotLinked(0));
    radio_sz = ImGui::GetItemRectSize();
    ImVec2 size = ImGui::CalcTextSize(islots[0].c_str(),
                                      islots[0].c_str() + islots[0].size());
    max_w = std::max(max_w, size.x);
    draw_list->AddText(pos + ImVec2{radio_sz.x, 0}, col, islots[0].c_str(),
                       islots[0].c_str() + islots[0].size());

    pos += ImVec2{0, std::max(radio_sz.y, size.y)};
    for (std::size_t i = 1; i < islots.size(); i++) {
      pos += ImVec2{0, padding};
      ImGui::SetCursorScreenPos(pos);
      ImGui::RadioButton("##", IsIslotLinked(i));
      ImVec2 size = ImGui::CalcTextSize(islots[i].c_str(),
                                        islots[i].c_str() + islots[i].size());
      max_w = std::max(max_w, size.x);
      draw_list->AddText(pos + ImVec2{radio_sz.x, 0}, col, islots[i].c_str(),
                         islots[i].c_str() + islots[i].size());
      pos += ImVec2{0, std::max(radio_sz.y, size.y)};
    }
    auto islots_p1 = pos + ImVec2{radio_sz.x + max_w, 0};
    return islots_p1 - islots_p0;
  }
  ImVec2 UpdateOslots(ImVec2 pos, ImVec2 radio_sz,
                      ImU32 col = IM_COL32(255, 255, 255, 255)) {
    if (oslots.empty()) {
      return {0, 0};
    }

    float max_w = 0;
    for (auto &slot : oslots) {
      ImVec2 size =
          ImGui::CalcTextSize(slot.c_str(), slot.c_str() + slot.size());
      max_w = std::max(max_w, size.x);
    }

    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    auto islots_p0 = pos;
    pos += ImVec2{max_w + radio_sz.x, 0};
    ImGui::SetCursorScreenPos(pos - ImVec2{radio_sz.x, 0});
    ImGui::RadioButton("##", IsOslotLinked(0));
    ImVec2 size = ImGui::CalcTextSize(oslots[0].c_str(),
                                      oslots[0].c_str() + oslots[0].size());
    draw_list->AddText(pos - ImVec2{radio_sz.x + size.x, 0}, col,
                       oslots[0].c_str(), oslots[0].c_str() + oslots[0].size());
    pos += ImVec2{0, std::max(radio_sz.y, size.y)};

    for (std::size_t i = 1; i < oslots.size(); i++) {
      pos += ImVec2{0, padding};
      ImGui::SetCursorScreenPos(pos - ImVec2{radio_sz.x, 0});
      ImGui::RadioButton("##", IsOslotLinked(i));
      ImVec2 size = ImGui::CalcTextSize(oslots[i].c_str(),
                                        oslots[i].c_str() + oslots[i].size());
      draw_list->AddText(pos - ImVec2{radio_sz.x + size.x, 0}, col,
                         oslots[i].c_str(),
                         oslots[i].c_str() + oslots[i].size());
      pos += ImVec2{0, std::max(radio_sz.y, size.y)};
    }
    return pos - islots_p0;
  }

public:
  NodeImpl(ImVec2 pos) : pos(pos), title("NewNode") {}

  void Update(ImU32 col = IM_COL32(255, 255, 255, 255)) {
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    auto node_p0_padding = pos;

    auto title_p0_padding = pos;
    auto title_sz = UpdateTitle(pos + ImVec2{padding, padding}, col);
    auto title_p1_padding =
        title_p0_padding + title_sz + ImVec2{padding * 2, padding * 2};

    ImVec2 radio_sz;
    auto islots_p0_padding = ImVec2{title_p0_padding.x, title_p1_padding.y};
    auto islots_sz = UpdateIslots(islots_p0_padding + ImVec2{padding, padding},
                                  radio_sz, col);
    auto islots_p1_padding =
        islots_p0_padding + islots_sz + ImVec2{padding * 2, padding * 2};
    // draw_list->AddRect(islots_p0_padding, islots_p1_padding, col, 4);

    auto oslots_p0_padding =
        ImVec2{islots_p0_padding.x + padding + islots_sz.x + padding * 4,
               islots_p0_padding.y};
    auto oslots_sz = UpdateOslots(oslots_p0_padding + ImVec2{padding, padding},
                                  radio_sz, col);
    auto oslots_p1_padding =
        oslots_p0_padding + oslots_sz + ImVec2{padding * 2, padding * 2};
    // draw_list->AddRect(oslots_p0_padding, oslots_p1_padding, col, 4);

    auto node_p1_padding =
        ImVec2{std::max(islots_p1_padding.x, oslots_p1_padding.x),
               std::max(islots_p1_padding.y, oslots_p1_padding.y)};
    draw_list->AddRect(node_p0_padding, node_p1_padding, col, 4);

    auto header_p0_padding = node_p0_padding;
    auto header_p1_padding = ImVec2{node_p1_padding.x, title_p1_padding.y};
    draw_list->AddLine(ImVec2{header_p0_padding.x, title_p1_padding.y},
                       header_p1_padding, col);
    ImGui::SetCursorScreenPos(header_p0_padding);
    ImGui::PushID(&title);
    ImGui::InvisibleButton("##", header_p1_padding - header_p0_padding,
                           ImGuiButtonFlags_MouseButtonLeft);
    ImGui::PopID();
    if (ImGui::IsItemActive() &&
        ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
      pos += io.MouseDelta;
    }
  }
};

class Node {
  std::unique_ptr<NodeImpl> impl;

public:
  Node(ImVec2 pos) : impl(std::make_unique<NodeImpl>(pos)) {}

  void Update() { impl->Update(); }
};
