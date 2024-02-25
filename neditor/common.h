#pragma once

#include <ostream>

#include "imgui.h"

inline ImVec2 operator+(const ImVec2 &a, const ImVec2 &b) {
  return ImVec2(a.x + b.x, a.y + b.y);
}
inline ImVec2 operator-(const ImVec2 &a, const ImVec2 &b) {
  return ImVec2(a.x - b.x, a.y - b.y);
}
inline ImVec2 &operator+=(ImVec2 &a, const ImVec2 &b) {
  a = a + b;
  return a;
}
inline ImVec2 &operator-=(ImVec2 &a, const ImVec2 &b) {
  a = a - b;
  return a;
}

inline std::ostream &operator<<(std::ostream &os, const ImVec2 &point) {
  os << "ImVec2{x=" << point.x << ", y=" << point.y << "}";
  return os;
}

inline void DrawBezierCubic(ImVec2 p1, ImVec2 p4,
                            ImU32 col = IM_COL32(255, 255, 255, 255),
                            float thickness = 1) {
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  auto p2 = p1 + ImVec2{(p4.x - p1.x) * 0.6f, 0};
  auto p3 = p4 - ImVec2{(p4.x - p1.x) * 0.6f, 0};
  draw_list->AddBezierCubic(p1, p2, p3, p4, col, thickness);
}
