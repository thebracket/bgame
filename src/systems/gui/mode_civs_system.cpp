#include "mode_civs_system.hpp"
#include "../../main/game_globals.hpp"
#include "../../messages/map_dirty_message.hpp"
#include "../../messages/recalculate_mining_message.hpp"
#include "../../utils/string_utils.hpp"
#include "gui_system.hpp"
#include <sstream>
#include "../../external/imgui-sfml/imgui-SFML.h"

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
    ImGui::Begin("Other Civilizations", nullptr, ImVec2{600,400}, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::ListBox("", &selected_civ, civ_listbox_items, civs.size(), 10);

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
    dialog_placement_t box{};

    std::unique_ptr<gui_dialog> dialog = std::make_unique<gui_dialog>(std::string(" [") + planet.civs.civs[negotiating_civ].name + std::string("] "), [] () { 
        // On close
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    });

    std::unique_ptr<gui_table> table = std::make_unique<gui_table>(box.left+1, box.top+3);

    // Salutation
    std::stringstream ss;
    ss << "Greetings, Cordex. We are The " << planet.civs.civs[negotiating_civ].name << ".";
    civs_add_text_line_to_table(box, table, box.w-2, ss.str());
    civs_add_text_line_to_table(box, table, box.w-2, " ");

    // Feelings line
    std::string feelings;
    bool show_bribe = false;
    bool show_war = false;

    if (planet.civs.civs[negotiating_civ].cordex_feelings < 0) {
        feelings = "You have invaded our lands, harassed our people, and must die.";
        show_bribe = true;
    } else if (planet.civs.civs[negotiating_civ].cordex_feelings == 0) {
        feelings = "You have occupied our lands, but perhaps we can work something out?";
        show_bribe = true;
        show_war = true;
    } else {
        feelings = "We welcome your presence here.";
        show_war = true;
    }
    civs_add_text_line_to_table(box, table, box.w-2, feelings);
    civs_add_text_line_to_table(box, table, box.w-2, " ");

    if (show_bribe) {
        civs_add_button_line_to_table(box, table, box.w-2, "<Maybe 50 Mcr will make you feel better?>", [] () {
            game_master_mode = PLAY;
            planet.civs.civs[negotiating_civ].cordex_feelings = 0;
            designations->current_cash -= 50;
        }, rltk::colors::GREEN, rltk::colors::DARKEST_GREEN);
        civs_add_text_line_to_table(box, table, box.w-2, " ");
    }
    if (show_war) {
        civs_add_button_line_to_table(box, table, box.w-2, "<I hope that you all die horribly in a fire!>", [] () {
            game_master_mode = PLAY;
            planet.civs.civs[negotiating_civ].cordex_feelings = -1;
        }, rltk::colors::RED, rltk::colors::DARKEST_GREEN);
        civs_add_text_line_to_table(box, table, box.w-2, " ");
    }
    // Show end negotiation
    civs_add_button_line_to_table(box, table, box.w-2, "<That's all for now>", [] () {
            game_master_mode = PLAY;
        }, rltk::colors::WHITE, rltk::colors::DARKEST_GREEN);
    civs_add_text_line_to_table(box, table, box.w-2, " ");

    dialog->children.push_back(std::move(table));

    add_gui_element(std::move(dialog));
}
