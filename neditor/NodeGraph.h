#pragma once

#include "Node.h"

class NodeGraph {
  std::list<Node> nodes;

public:
  void NewNode() { nodes.emplace_back(ImVec2{100, 100}); }
  void Reset() { nodes.clear(); }

  void Update() {
    for (auto &node : nodes) {
      node.UpdateLinks();
    }
    for (auto &node : nodes) {
      node.UpdateNode();
    }
    Node::FinishUpdate();
  }
};
