#pragma once

#include <cmath>

#include "imgui.h"

#include "common.h"

class NodeGraph {
  constexpr static float GRID_STEP{64.0f};

  constexpr static ImU32 color_cbackground = IM_COL32(50, 50, 50, 255);
  constexpr static ImU32 color_cforeground = IM_COL32(200, 200, 200, 40);

  ImVec2 canvas_p0;
  ImVec2 canvas_sz;
  ImVec2 scrolling{0.f, 0.f};

  void UpdateCanvas() {
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    canvas_p0 = ImGui::GetCursorScreenPos();
    canvas_sz = ImGui::GetContentRegionAvail();

    draw_list->AddRectFilled(canvas_p0, canvas_p0 + canvas_sz,
                             color_cbackground);
    draw_list->AddRect(canvas_p0, canvas_p0 + canvas_sz, color_cforeground);
    for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x;
         x += GRID_STEP) {
      draw_list->AddLine(canvas_p0 + ImVec2{x, 0},
                         canvas_p0 + ImVec2{x, canvas_sz.y}, color_cforeground);
    }
    for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y;
         y += GRID_STEP) {
      draw_list->AddLine(canvas_p0 + ImVec2{0, y},
                         canvas_p0 + ImVec2{canvas_sz.x, y}, color_cforeground);
    }
    ImGui::InvisibleButton("##", canvas_sz);
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

    UpdateCanvas();
    ImGui::PushClipRect(canvas_p0, canvas_p0 + canvas_sz, true);
    ImGui::PopClipRect();

    ImGui::End();
  }

  void NewNode() {
    // TODO: impl
  }
};
