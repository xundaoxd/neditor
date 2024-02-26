#pragma once

#include "Node.h"

class NodeGraph {
  std::list<Node> nodes;

public:
  void NewNode() { nodes.emplace_back(ImVec2{100, 100}); }
  void Reset() { nodes.clear(); }

  void Update() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("NodeGraph");
    ImGui::PopStyleVar(3);
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
