#pragma once

#include "Node.h"
#include "imgui.h"

struct NodeProperty {
  static bool is_editing;
  static char edit_buf[64];
  static int edit_target; // 0 for title, 1 for i slot, 2 for o slot
  static std::size_t edit_slot;

  void UpdateTitle(Node *node) {
    ImGui::Spacing();
    if (is_editing && edit_target == 0) {
      if (ImGui::InputText("##", edit_buf, sizeof(edit_buf),
                           ImGuiInputTextFlags_EnterReturnsTrue)) {
        node->title = edit_buf;
        memset(edit_buf, 0, sizeof(edit_buf));
        is_editing = false;
      }
    } else {
      ImGui::TextUnformatted(node->title.c_str(),
                             node->title.c_str() + node->title.size());
      if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
        edit_target = 0;
        strncpy(edit_buf, node->title.c_str(), node->title.size());
        is_editing = true;
      }
    }
    ImGui::Spacing();
  }

  void UpdateSlots(const char *label, std::list<Slot> &slots, int n) {
    if (ImGui::CollapsingHeader(label)) {
      std::size_t del_idx = -1;
      std::size_t idx = 0;
      ImGui::Spacing();
      ImGui::Button("New");
      if (ImGui::IsItemHovered() &&
          ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        slots.emplace_back("New");
      }
      ImGui::Spacing();
      for (auto &slot : slots) {
        if (is_editing && edit_target == n && edit_slot == idx) {
          if (ImGui::InputText("##", edit_buf, sizeof(edit_buf),
                               ImGuiInputTextFlags_EnterReturnsTrue)) {
            slot.name = edit_buf;
            memset(edit_buf, 0, sizeof(edit_buf));
            is_editing = false;
          }
        } else {
          ImGui::TextUnformatted(slot.name.c_str(),
                                 slot.name.c_str() + slot.name.size());
          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            edit_slot = idx;
            edit_target = n;
            strncpy(edit_buf, slot.name.c_str(), slot.name.size());
            is_editing = true;
          }
          ImGui::SameLine();
          ImGui::Button("Del");
          if (ImGui::IsItemHovered() &&
              ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            del_idx = idx;
          }
        }
        ImGui::Spacing();
        idx++;
      }
      if (del_idx != -1ul) {
        auto it = std::next(slots.begin(), del_idx);
        it->UnLink();
        slots.erase(it);
      }
    }
  }

  void Update() {
    Node *node = Node::GetSelectedNode();
    if (!node) {
      return;
    }
    UpdateTitle(node);
    UpdateSlots("Input SLots", node->islots, 1);
    UpdateSlots("Output SLots", node->oslots, 2);
  }
};

inline bool NodeProperty::is_editing{false};
inline char NodeProperty::edit_buf[64];
inline int NodeProperty::edit_target;
inline std::size_t NodeProperty::edit_slot;
