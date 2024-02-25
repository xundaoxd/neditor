#pragma once

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
  std::vector<std::string> islots;
  std::vector<std::string> oslots;

  std::vector<std::pair<NodeImpl *, std::size_t>> links;

  ImVec2 UpdateTitle(ImVec2 pos, ImU32 col = IM_COL32(255, 255, 255, 255)) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddText(pos, col, title.c_str(), title.c_str() + title.size());
    ImVec2 size =
        ImGui::CalcTextSize(title.c_str(), title.c_str() + title.size());
    return size;
  }

public:
  NodeImpl(ImVec2 pos) : pos(pos), title("NewNode") {}

  void Update(ImU32 col = IM_COL32(255, 255, 255, 255)) {
    auto title_p0_padding = pos;
    auto title_sz = UpdateTitle(pos + ImVec2{padding, padding});
    auto title_p1_padding =
        title_p0_padding + title_sz + ImVec2{padding * 2, padding * 2};

    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRect(title_p0_padding, title_p1_padding, col);
  }
};

class Node {
  std::unique_ptr<NodeImpl> impl;

public:
  Node(ImVec2 pos) : impl(std::make_unique<NodeImpl>(pos)) {}

  void Update() { impl->Update(); }
};
