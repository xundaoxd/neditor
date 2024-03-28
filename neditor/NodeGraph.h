#pragma once

#include <cmath>
#include <list>
#include <numeric>

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
  constexpr static float padding{4.0f};
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
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
      scrolling += io.MouseDelta;
    }
    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right)) {
      scrolling = ImVec2{0, 0};
    }
  }

  void UpdateNode(Node *node) {
    // TODO: impl
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    ImVec2 slot_sz = ImGui::CalcTextSize(
        node->name.c_str(), node->name.c_str() + node->name.size());
    slot_sz = std::accumulate(
        node->islots.begin(), node->islots.end(), slot_sz,
        [](const auto &ret, const auto &slot) {
          auto sz = ImGui::CalcTextSize(slot.name.c_str(),
                                        slot.name.c_str() + slot.name.size());
          sz.x += sz.y;
          return ImVec2{std::max(ret.x, sz.x), std::max(ret.y, sz.y)};
        });
    slot_sz = std::accumulate(
        node->oslots.begin(), node->oslots.end(), slot_sz,
        [](const auto &ret, const auto &slot) {
          auto sz = ImGui::CalcTextSize(slot.name.c_str(),
                                        slot.name.c_str() + slot.name.size());
          sz.x += sz.y;
          return ImVec2{std::max(ret.x, sz.x), std::max(ret.y, sz.y)};
        });

    auto header_p0 = node->pos;
    auto title_p0 = header_p0 + padding;
    auto title_p1 =
        title_p0 + ImGui::CalcTextSize(node->name.c_str(),
                                       node->name.c_str() + node->name.size());
    auto header_p1 = header_p0 + padding * 2 +
                     ImVec2{slot_sz.x * 2 + padding * 4, slot_sz.y};
    draw_list->AddRectFilled(header_p0 + scrolling, header_p1 + scrolling,
                             color_hbackground);
    draw_list->AddText(title_p0 + scrolling, color_hforeground,
                       node->name.c_str(),
                       node->name.c_str() + node->name.size());
    ImGui::SetCursorScreenPos(header_p0 + scrolling);
    ImGui::InvisibleButton(node->name.c_str(), header_p1 - header_p0,
                           ImGuiButtonFlags_MouseButtonLeft);
    if (ImGui::IsItemActive() &&
        ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
      node->pos += io.MouseDelta;
    }

    {
      // draw islots
      auto slot_p0 = ImVec2{header_p0.x, header_p1.y};
      auto cur_pos = slot_p0 + padding;
      for (const auto &islot : node->islots) {
        ImGui::SetCursorScreenPos(cur_pos + scrolling);
        ImGui::SetNextWindowSize(slot_sz);
        ImGui::RadioButton(islot.name.c_str(), false);
        cur_pos += ImVec2{0, slot_sz.y + padding};
      }
    }
  }

public:
  void Update(const char *name) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(name);
    ImGui::PopStyleVar(3);

    ImGui::PushClipRect(canvas_p0, canvas_p0 + canvas_sz, true);
    UpdateCanvas();
    for (auto &&node : nodes) {
      UpdateNode(&node);
    }
    ImGui::PopClipRect();

    ImGui::End();
  }

  void NewNode() {
    nodes.emplace_back(
        Node{"NewNode", ImVec2{200, 200}, {{"demo"}, {"demo1"}}, {}});
  }
};
