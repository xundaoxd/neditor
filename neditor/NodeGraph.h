#pragma once

#include <cmath>
#include <list>

#include "imgui.h"

#include "common.h"

struct Slot {
  std::string name;
  ImVec2 pos;
};

struct Node {
  std::string name;
  ImVec2 pos;
  std::list<Slot> islots, oslots;
};

class NodeGraph {
  constexpr static float GRID_STEP{64.0f};

  constexpr static ImU32 color_background = IM_COL32(50, 50, 50, 255);
  constexpr static ImU32 color_foreground = IM_COL32(200, 200, 200, 40);

  constexpr static ImU32 color_nbackground = IM_COL32(100, 100, 100, 255);
  constexpr static ImU32 color_nforeground = IM_COL32(200, 200, 200, 255);

  constexpr static ImU32 color_hbackground = IM_COL32(0, 127, 255, 255);
  constexpr static ImU32 color_hforeground = IM_COL32(200, 200, 200, 255);

  ImVec2 canvas_p0;
  ImVec2 canvas_sz;
  ImVec2 scrolling{0.f, 0.f};

  std::list<Node> nodes;

  void UpdateCanvas() {
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    canvas_p0 = ImGui::GetCursorScreenPos();
    canvas_sz = ImGui::GetContentRegionAvail();

    draw_list->AddRectFilled(canvas_p0, canvas_p0 + canvas_sz,
                             color_background);
    draw_list->AddRect(canvas_p0, canvas_p0 + canvas_sz, color_foreground);
    for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x;
         x += GRID_STEP) {
      draw_list->AddLine(canvas_p0 + ImVec2{x, 0},
                         canvas_p0 + ImVec2{x, canvas_sz.y}, color_foreground);
    }
    for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y;
         y += GRID_STEP) {
      draw_list->AddLine(canvas_p0 + ImVec2{0, y},
                         canvas_p0 + ImVec2{canvas_sz.x, y}, color_foreground);
    }
    ImGui::PushID(this);
    ImGui::InvisibleButton("##", canvas_sz, ImGuiButtonFlags_MouseButtonRight);
    ImGui::PopID();
    if (ImGui::IsItemActive() &&
        ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
      scrolling += io.MouseDelta;
    }
    if (ImGui::IsItemActive() &&
        ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right)) {
      scrolling = ImVec2{0, 0};
    }
  }

  void UpdateNode(Node *node) {
    // TODO: impl
  }

public:
  void Update(const char *name) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(name);
    ImGui::PopStyleVar(3);

    UpdateCanvas();
    ImGui::PushClipRect(canvas_p0, canvas_p0 + canvas_sz, true);
    for (auto &&node : nodes) {
      UpdateNode(&node);
    }
    ImGui::PopClipRect();

    ImGui::End();
  }

  void NewNode() { nodes.emplace_back(Node{"NewNode", ImVec2{200, 200}}); }
};
