#include "mode_civs_system.hpp"
#include "../../main/game_globals.hpp"
#include "../../messages/map_dirty_message.hpp"
#include "../../messages/recalculate_mining_message.hpp"
#include "../../utils/string_utils.hpp"
#include "gui_system.hpp"
#include <sstream>
#include "../../external/imgui-sfml/imgui-SFML.h"
#include "imgui_helper.hpp"

using namespace rltk;
using namespace rltk::colors;

void mode_civs_system::configure() {
    system_name = "Civilizations System";
    subscribe_mbox<key_pressed_t>();
}

void render_civ_list();
void render_civ_negotiate();

void mode_civs_system::update(const double ms) {
    if (game_master_mode != CIVS && game_master_mode != CIV_NEGOTIATE) return;

    if (game_master_mode == CIVS) {
        render_civ_list();
    } else if (game_master_mode == CIV_NEGOTIATE) {
        render_civ_negotiate();
    }
}

int selected_civ = 0;

void render_civ_list() {
    std::vector<std::pair<std::size_t, std::string>> civs;
    std::size_t civ_id = 0;
    for (civ_t &civ : planet.civs.civs) {
        if (civ.met_cordex) {
            std::string line = civ.name + std::string(" ");
            if (civ.cordex_feelings < 0) {
                line += "(WAR)";
            } else if (civ.cordex_feelings == 0) {
                line += "(NEUTRAL)";
            } else {
                line += "(FRIENDLY)";
            }
            civs.emplace_back(std::make_pair(civ_id, line));
        }
        ++civ_id;
    }
    const char* civ_listbox_items[civs.size()];
    for (int i=0; i<civs.size(); ++i) {
        civ_listbox_items[i] = civs[i].second.c_str();
    }
    ImGui::Begin(win_civs_list.c_str(), nullptr, ImVec2{600,400}, 0.9f, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
    ImGui::PushItemWidth(-1);
    ImGui::ListBox("## ", &selected_civ, civ_listbox_items, civs.size(), 10);

    if (ImGui::Button("Negotiate")) {
        negotiating_civ = civs[selected_civ].first;
        game_master_mode = CIV_NEGOTIATE;
    }

    ImGui::SameLine();
    if (ImGui::Button("Close")) {
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }

    ImGui::End();
}

void civs_add_text_line_to_table(const dialog_placement_t &box, std::unique_ptr<gui_table> &table, const int &w, const std::string &s)
{
    gui_table_row row;
    std::unique_ptr<gui_table_text> c_line = std::make_unique<gui_table_text>(box.right - 2, s);
    row.cols.push_back(std::move(c_line));
    table->rows.push_back(std::move(row));
}

void civs_add_button_line_to_table(const dialog_placement_t &box, std::unique_ptr<gui_table> &table, const int &w, const std::string &s,
    const std::function<void()> &on_click, const rltk::color_t &fg, const rltk::color_t &bg)
{
    gui_table_row row;
    std::unique_ptr<gui_table_button> c_line = std::make_unique<gui_table_button>(box.right - 2, s, on_click);
    c_line->colors = std::make_pair(fg, bg);
    row.cols.push_back(std::move(c_line));
    table->rows.push_back(std::move(row));
}

void render_civ_negotiate() {
    const std::string title = std::string("Negotiations with ") + planet.civs.civs[negotiating_civ].name;
    const std::string salutation = std::string("Greeting, Cordex. We are The ") + planet.civs.civs[negotiating_civ].name;
    std::string feelings;
    ImVec4 feelings_color{1.0f, 1.0f, 1.0f, 1.0f};
    bool show_bribe = false;
    bool show_war = false;

    if (planet.civs.civs[negotiating_civ].cordex_feelings < 0) {
        feelings = "You have invaded our lands, harassed our people, and must die.";
        feelings_color = ImVec4{1.0f, 0.0f, 0.0f, 1.0f};
        show_bribe = true;
    } else if (planet.civs.civs[negotiating_civ].cordex_feelings == 0) {
        feelings = "You have occupied our lands, but perhaps we can work something out?";
        feelings_color = ImVec4{1.0f, 1.0f, 0.0f, 1.0f};
        show_bribe = true;
        show_war = true;
    } else {
        feelings = "We welcome your presence here, and look forward to working with you.";
        feelings_color = ImVec4{0.0f, 1.0f, 0.0f, 1.0f};
        show_war = true;
    }

    ImGui::Begin(title.c_str());
    ImGui::Text(salutation.c_str());
    ImGui::TextColored(feelings_color, feelings.c_str());
    if (show_bribe) {
        if (ImGui::Button("Maybe 50 Mcr will make you feel better?")) {
            game_master_mode = PLAY;
            planet.civs.civs[negotiating_civ].cordex_feelings = 0;
            designations->current_cash -= 50;
        }
    }
    if (show_war) {
        if (ImGui::Button("I hope that you all die horribly in a fire!")) {
            game_master_mode = PLAY;
            planet.civs.civs[negotiating_civ].cordex_feelings = -1;
        }
    }
    if (ImGui::Button("End Negotiation")) {
        game_master_mode = PLAY;
    }
    ImGui::End();
}
