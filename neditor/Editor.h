#pragma once
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
  Node node{"demo", {}, {}};

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
    // static ImVector<ImVec2> points;
    // static bool adding_line = false;
    //
    // ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
    // ImVec2 canvas_p1 = canvas_p0 + ImGui::GetContentRegionAvail();
    //
    // ImGuiIO &io = ImGui::GetIO();
    // ImDrawList *draw_list = ImGui::GetWindowDrawList();
    //
    // draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50,
    // 255)); draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255,
    // 255));
    //
    // ImGui::InvisibleButton("## canvas", canvas_p1 - canvas_p0,
    //                        ImGuiButtonFlags_MouseButtonLeft |
    //                            ImGuiButtonFlags_MouseButtonRight);
    // ImVec2 mouse_pos_in_canvas = io.MousePos - canvas_p0;
    // if (ImGui::IsItemHovered() && !adding_line &&
    //     ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    //   points.push_back(mouse_pos_in_canvas);
    //   points.push_back(mouse_pos_in_canvas);
    //   adding_line = true;
    // }
    // if (adding_line) {
    //   points.back() = mouse_pos_in_canvas;
    //   if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
    //     adding_line = false;
    // }
    // for (int n = 0; n < points.Size; n += 2)
    //   draw_list->AddLine(canvas_p0 + points[n], canvas_p0 + points[n + 1],
    //                      IM_COL32(255, 255, 0, 255), 2.0f);
    // if (ImGui::IsMouseDragging(0)) {
    //   std::cout << io.MouseDelta << std::endl;
    // }
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

    ImGui::End();
  }
};
