#pragma once

#include <cmath>

#include "imgui.h"

#include "common.h"

class NodeGraph {
  ImVec2 scrolling{0.f, 0.f};
  const float GRID_STEP{64.0f};

  void UpdateBackground() {
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    ImVec2 p0 = ImGui::GetCursorScreenPos();
    ImVec2 size = ImGui::GetContentRegionAvail();

    draw_list->AddRectFilled(p0, p0 + size, IM_COL32(50, 50, 50, 255));
    for (float x = fmodf(scrolling.x, GRID_STEP); x < size.x; x += GRID_STEP) {
      draw_list->AddLine(p0 + ImVec2{x, 0}, p0 + ImVec2{x, size.y},
                         IM_COL32(200, 200, 200, 40));
    }
    for (float y = fmodf(scrolling.y, GRID_STEP); y < size.y; y += GRID_STEP) {
      draw_list->AddLine(p0 + ImVec2{0, y}, p0 + ImVec2{size.x, y},
                         IM_COL32(200, 200, 200, 40));
    }
    ImGui::InvisibleButton("##", size);
    if (ImGui::IsItemHovered() &&
        ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
      scrolling += io.MouseDelta;
    }
    if (ImGui::IsItemHovered() &&
        ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right)) {
      scrolling = ImVec2{0, 0};
    }
  }

public:
  void Update(const char *name) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(name);
    ImGui::PopStyleVar(3);

    UpdateBackground();

    ImGui::End();
  }
};
