#pragma once

#include "imgui.h"

#include "common.h"

class NodeGraph {

public:
  void Update(const char *name) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(name);
    ImGui::PopStyleVar(3);

    ImGui::End();
  }
};
