#pragma once

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "imgui.h"

#include "common.h"

class Node {
  static constexpr float padding = 6;

  static Node *selected_node;

  struct Slot {
    std::string name;
    ImVec2 pos;

    std::list<Slot *> links;

    Slot(std::string name, ImVec2 pos)
        : name(std::move(name)), pos(std::move(pos)) {}
    Slot(std::string name) : Slot(std::move(name), {0, 0}) {}
  };

  ImVec2 pos;

  std::string title;
  std::list<Slot> islots{{"demo"}, {"asd"}};
  std::list<Slot> oslots{{"asd"}, {"123"}};

public:
  Node(ImVec2 pos) : pos(pos), title("NewNode") {}

  Slot &GetIslot(std::size_t idx) { return *std::next(islots.begin(), idx); }
  Slot &GetOslot(std::size_t idx) { return *std::next(oslots.begin(), idx); }
  const Slot &GetIslot(std::size_t idx) const {
    return *std::next(islots.begin(), idx);
  }
  const Slot &GetOslot(std::size_t idx) const {
    return *std::next(oslots.begin(), idx);
  }

  bool IsSlotLinked(const Slot *slot) const { return slot->links.size(); }
  bool IsIslotLinked(std::size_t idx) const {
    return GetIslot(idx).links.size();
  }
  bool IsOslotLinked(std::size_t idx) const {
    return GetOslot(idx).links.size();
  }

  bool IsSelected() const { return this == Node ::selected_node; }

  ImVec2 UpdateTitle(ImVec2 pos, ImU32 col = IM_COL32(255, 255, 255, 255)) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddText(pos, col, title.c_str(), title.c_str() + title.size());
    auto size =
        ImGui::CalcTextSize(title.c_str(), title.c_str() + title.size());
    return size;
  }

  ImVec2 UpdateIslot(ImVec2 pos, std::size_t idx, ImVec2 &radio_sz,
                     ImU32 col = IM_COL32(255, 255, 255, 255)) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    Slot &slot = GetIslot(idx);

    ImGui::SetCursorScreenPos(pos);
    ImGui::RadioButton("##", IsSlotLinked(&slot));
    radio_sz = ImGui::GetItemRectSize();
    ImVec2 size = ImGui::CalcTextSize(slot.name.c_str(),
                                      slot.name.c_str() + slot.name.size());
    draw_list->AddText(pos + ImVec2{radio_sz.x, 0}, col, slot.name.c_str(),
                       slot.name.c_str() + slot.name.size());
    slot.pos = pos + ImVec2{radio_sz.x / 2, radio_sz.y / 2};
    return ImVec2{radio_sz.x + size.x, std::max(radio_sz.y, size.y)};
  }
  ImVec2 UpdateOslot(ImVec2 pos, std::size_t idx, ImVec2 &radio_sz,
                     ImU32 col = IM_COL32(255, 255, 255, 255)) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    Slot &slot = GetOslot(idx);

    ImGui::SetCursorScreenPos(pos - ImVec2{radio_sz.x, 0});
    ImGui::RadioButton("##", IsSlotLinked(&slot));
    ImVec2 size = ImGui::CalcTextSize(slot.name.c_str(),
                                      slot.name.c_str() + slot.name.size());
    draw_list->AddText(pos - ImVec2{radio_sz.x + size.x, 0}, col,
                       slot.name.c_str(), slot.name.c_str() + slot.name.size());
    slot.pos = pos + ImVec2{-radio_sz.x / 2, radio_sz.y / 2};
    return ImVec2{size.x + radio_sz.x, std::max(size.y, radio_sz.y)};
  }

  ImVec2 UpdateIslots(ImVec2 pos, ImVec2 &radio_sz,
                      ImU32 col = IM_COL32(255, 255, 255, 255)) {
    if (islots.empty()) {
      return {0, 0};
    }
    auto islots_p0 = pos;
    float max_w = 0;

    auto islot_sz = UpdateIslot(pos, 0, radio_sz, col);
    max_w = std::max(max_w, islot_sz.x);
    pos += ImVec2{0, islot_sz.y};

    for (std::size_t i = 1; i < islots.size(); i++) {
      pos += ImVec2{0, padding};

      auto islot_sz = UpdateIslot(pos, i, radio_sz, col);
      max_w = std::max(max_w, islot_sz.x);
      pos += ImVec2{0, islot_sz.y};
    }
    auto islots_p1 = pos + ImVec2{max_w, 0};
    return islots_p1 - islots_p0;
  }

  ImVec2 UpdateOslots(ImVec2 pos, ImVec2 radio_sz,
                      ImU32 col = IM_COL32(255, 255, 255, 255)) {
    if (oslots.empty()) {
      return {0, 0};
    }

    float max_w = 0;
    for (auto &slot : oslots) {
      ImVec2 size = ImGui::CalcTextSize(slot.name.c_str(),
                                        slot.name.c_str() + slot.name.size());
      max_w = std::max(max_w, size.x + radio_sz.x);
    }

    auto oslots_p0 = pos;
    pos += ImVec2{max_w, 0};

    auto oslot_sz = UpdateOslot(pos, 0, radio_sz, col);
    pos += ImVec2{0, oslot_sz.y};

    for (std::size_t i = 1; i < oslots.size(); i++) {
      pos += ImVec2{0, padding};

      auto oslot_sz = UpdateOslot(pos, i, radio_sz, col);
      pos += ImVec2{0, oslot_sz.y};
    }
    return pos - oslots_p0;
  }

  void UpdateLinks() {
    for (auto &slot : islots) {
      for (auto &src : slot.links) {
        DrawBezierCubic(src->pos, slot.pos);
      }
    }
  }

  void UpdateNode(ImU32 col = IM_COL32(255, 255, 255, 255)) {
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    auto node_p0_padding = pos;

    auto title_p0_padding = pos;
    auto title_sz = UpdateTitle(pos + ImVec2{padding, padding}, col);
    auto title_p1_padding =
        title_p0_padding + title_sz + ImVec2{padding * 2, padding * 2};

    ImVec2 radio_sz;
    auto islots_p0_padding = ImVec2{title_p0_padding.x, title_p1_padding.y};
    auto islots_sz = UpdateIslots(islots_p0_padding + ImVec2{padding, padding},
                                  radio_sz, col);
    auto islots_p1_padding =
        islots_p0_padding + islots_sz + ImVec2{padding * 2, padding * 2};
    // draw_list->AddRect(islots_p0_padding, islots_p1_padding, col, 4);

    auto oslots_p0_padding =
        ImVec2{islots_p0_padding.x + padding + islots_sz.x + padding * 4,
               islots_p0_padding.y};
    auto oslots_sz = UpdateOslots(oslots_p0_padding + ImVec2{padding, padding},
                                  radio_sz, col);
    auto oslots_p1_padding =
        oslots_p0_padding + oslots_sz + ImVec2{padding * 2, padding * 2};
    // draw_list->AddRect(oslots_p0_padding, oslots_p1_padding, col, 4);

    auto node_p1_padding =
        ImVec2{std::max(islots_p1_padding.x, oslots_p1_padding.x),
               std::max(islots_p1_padding.y, oslots_p1_padding.y)};
    draw_list->AddRect(node_p0_padding, node_p1_padding, col, 4);

    auto header_p0_padding = node_p0_padding;
    auto header_p1_padding = ImVec2{node_p1_padding.x, title_p1_padding.y};
    draw_list->AddLine(ImVec2{header_p0_padding.x, title_p1_padding.y},
                       header_p1_padding, col);

    {
      ImGui::SetCursorScreenPos(header_p0_padding);
      ImGui::PushID(&title);
      ImGui::InvisibleButton("##", header_p1_padding - header_p0_padding,
                             ImGuiButtonFlags_MouseButtonLeft);
      ImGui::PopID();
      if (ImGui::IsItemHovered()) {
        draw_list->AddRectFilled(header_p0_padding, header_p1_padding,
                                 (col & ~(0xff << IM_COL32_A_SHIFT)) |
                                     (50 << IM_COL32_A_SHIFT));
      }
      if (ImGui::IsItemActive() &&
          ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        pos += io.MouseDelta;
      }
      if (ImGui::IsItemActive() &&
          ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        selected_node = this;
      }
    }
  }
};

inline Node *Node::selected_node{nullptr};
