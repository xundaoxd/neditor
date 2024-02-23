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

inline std::ostream &operator<<(std::ostream &os, const ImVec2 &point) {
  os << "ImVec2{x=" << point.x << ", y=" << point.y << "}";
  return os;
}

struct Node {
  std::string title;
  std::vector<std::string> in_slots;
  std::vector<std::string> out_slots;

  Node(std::string title, std::vector<std::string> in_slots,
       std::vector<std::string> out_slots)
      : title(std::move(title)), in_slots(std::move(in_slots)),
        out_slots(std::move(out_slots)) {}

  Node(std::string title) : Node(std::move(title), {}, {}) {}
};

class NodeEditor {
  static constexpr float padding = 6;
  static constexpr float mark = 6;

  struct NodeP {
    std::unique_ptr<Node> node{nullptr};
    ImVec2 pos{0, 0};

    NodeP(std::unique_ptr<Node> node, ImVec2 pos)
        : node(std::move(node)), pos(std::move(pos)) {}
  };

  ImVec2 scrolling{0, 0};
  std::vector<NodeP> nodes;

public:
  NodeEditor() {
    // TODO: test data
    nodes.emplace_back(std::make_unique<Node>("demo"), ImVec2{100, 100});
    nodes.back().node->in_slots = {"in0", "in1"};
    nodes.back().node->out_slots = {"out0", "out1"};
  }
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

  void DrawCanvas() {
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
    ImVec2 canvas_p1 = canvas_p0 + canvas_sz;

    ImGui::InvisibleButton("## canvas", canvas_p1 - canvas_p0,
                           ImGuiButtonFlags_MouseButtonLeft |
                               ImGuiButtonFlags_MouseButtonRight);
    if (ImGui::IsItemActive() &&
        ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
      scrolling = scrolling + io.MouseDelta;
    }

    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    {
      const float GRID_STEP = 64.0f;
      for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x;
           x += GRID_STEP)
        draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y),
                           ImVec2(canvas_p0.x + x, canvas_p1.y),
                           IM_COL32(200, 200, 200, 40));
      for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y;
           y += GRID_STEP)
        draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y),
                           ImVec2(canvas_p1.x, canvas_p0.y + y),
                           IM_COL32(200, 200, 200, 40));
    }
    // TODO: impl
  }

  // void DrawDrag() {
  //   static ImVec2 pos{100, 100};
  //   ImGui::SetCursorPos(pos);
  //   ImGui::Button("demo");
  //   ImGuiIO &io = ImGui::GetIO();
  //   if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
  //     pos = pos + io.MouseDelta;
  //   }
  // }

  // | <---padding---> elem <---padding--->|
  void DrawNodes() {
    // ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    ImU32 white_col = ImGui::GetColorU32(IM_COL32(255, 255, 255, 255));

    for (auto &node : nodes) {
      ImVec2 node_p0 = node.pos;
      ImVec2 cur_p0 = node_p0;

      cur_p0 = cur_p0 + ImVec2{0, padding};
      {
        // draw title
        ImVec2 title_sz = ImGui::CalcTextSize(node.node->title.c_str(),
                                              node.node->title.c_str() +
                                                  node.node->title.size());
        ImVec2 title_p0 = cur_p0 + ImVec2{padding, padding};
        ImVec2 title_p1 = title_p0 + title_sz;
        draw_list->AddText(title_p0, white_col, node.node->title.c_str(),
                           node.node->title.c_str() + node.node->title.size());

        cur_p0 = ImVec2{cur_p0.x, title_p1.y};
      }
      {
        // draw seperator
        cur_p0 = cur_p0 + ImVec2{0, 10};
      }
      {
        // draw slots
        // |<---padding---> islot <---2 * padding---> oslot <---padding--->|
        // islot: |mark <---padding---> slot|
        // oslot: |slot <---padding---> mark|
      }
      {
        // draw outline
      }
      // TODO: impl
    }
  }

  void DrawConns() {
    // TODO: impl
  }

  void DrawLoop() {
    ImGui::Begin("neditor", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

    DrawMenuBar();
    DrawPopup();

    DrawCanvas();
    DrawNodes();
    DrawConns();

    // ImGui::ShowDemoWindow();

    ImGui::End();
  }
};
