#pragma once

#include "imgui.h"
#include <ostream>

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
