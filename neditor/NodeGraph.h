#pragma once

#include "Node.h"

class NodeGraph {
  std::list<Node> nodes;

public:
  void NewNode() { nodes.emplace_back(ImVec2{100, 100}); }
  void Reset() { nodes.clear(); }

  void UpdateBackground() {
    auto p0 = ImGui::GetWindowPos();
    auto p1 = p0 + ImGui::GetWindowSize();

    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(p0, p1, IM_COL32(50, 50, 50, 40));

    const float GRID_STEP = 64.0f;
    for (auto x = p0.x; x < p1.x; x += GRID_STEP) {
      draw_list->AddLine(ImVec2(x, p0.y), ImVec2(x, p1.y),
                         IM_COL32(200, 200, 200, 40));
    }
    for (auto y = p0.y; y < p1.y; y += GRID_STEP) {
      draw_list->AddLine(ImVec2(p0.x, y), ImVec2(p1.x, y),
                         IM_COL32(200, 200, 200, 40));
    }
  }
  void Update(const char *name) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(name);
    ImGui::PopStyleVar(3);

    UpdateBackground();

    for (auto &node : nodes) {
      node.UpdateLinks();
    }
    for (auto &node : nodes) {
      node.UpdateNode();
    }
    Node::FinishUpdate();
    ImGui::End();
  }
};
