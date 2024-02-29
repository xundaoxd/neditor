#pragma once

#include "imgui.h"

struct NodeProperty {
  void Update(const char *name) {
    ImGui::Begin(name);
    ImGui::End();
  }
};
