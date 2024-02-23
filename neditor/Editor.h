#pragma once
#include <cmath>
#include <iostream>
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
};

class NodeEditor {
  ImVec2 scrolling{0, 0};

public:
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

  void DrawNodes() {
    // TODO: impl
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

    ImGui::ShowDemoWindow();

    ImGui::End();
  }
};
