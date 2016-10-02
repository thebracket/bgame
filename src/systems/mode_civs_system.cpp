#include "mode_civs_system.hpp"
#include "../main/game_globals.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../messages/recalculate_mining_message.hpp"
#include "../utils/string_utils.hpp"
#include "gui_system.hpp"
#include <sstream>

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

void render_civ_list() {
    const int term_w = rltk::term(3)->term_width;
    const int term_h = rltk::term(3)->term_height;

    const int box_left = 3;
    const int box_right = term_w - 3;
    const int box_top = 2;
    const int box_bottom = term_h - 2;

    std::unique_ptr<gui_dialog> dialog = std::make_unique<gui_dialog>(" Active Civilizations ", [] () { 
        // On close
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    });

    std::unique_ptr<gui_table> table = std::make_unique<gui_table>(box_left+1, box_top+3);
    std::size_t civ_id = 0;
    for (civ_t &civ : planet.civs.civs) {
        if (civ.met_cordex) {
            gui_table_row row;

            // Civ feelings
            std::unique_ptr<gui_table_text> c_feel;
            if (civ.cordex_feelings < 0) {
                c_feel = std::make_unique<gui_table_text>(8,"   WAR   ", WHITE, RED);
            } else if (civ.cordex_feelings == 0) {
                c_feel = std::make_unique<gui_table_text>(8, "NEUTRAL ", BLACK, CYAN);
            } else {
                c_feel = std::make_unique<gui_table_text>(8, "FRIENDLY", WHITE, GREEN);
            }
            row.cols.push_back(std::move(c_feel));

            // Negotiate button
            std::unique_ptr<gui_table_button> go_btn = std::make_unique<gui_table_button>(9, "Negotiate", [civ_id] () {
                game_master_mode = CIV_NEGOTIATE;
                negotiating_civ = civ_id;
            });
            row.cols.push_back(std::move(go_btn));

            // Civ name
            std::unique_ptr<gui_table_text> c_name = std::make_unique<gui_table_text>(box_right - 19, civ.name);
            row.cols.push_back(std::move(c_name));

            table->rows.push_back(std::move(row));
        }
        ++civ_id;
    }
    dialog->children.push_back(std::move(table));

    add_gui_element(std::move(dialog));
}

void render_civ_negotiate() {
    int font_width, font_height;
    std::tie(font_width, font_height) = term(1)->get_font_size();
    const int term_w = term(1)->term_width;
    const int term_h = term(1)->term_height;

    int mouse_x, mouse_y;
	std::tie(mouse_x, mouse_y) = get_mouse_position();

    const int terminal_x = mouse_x / font_width;
    const int terminal_y = mouse_y / font_height;

    const int box_left = 3;
    const int box_right = term_w - 3;
    const int box_top = 2;
    const int box_bottom = term_h - 2;

    // Panel and header
    term(1)->fill(box_left, box_top, box_right, box_bottom, ' ');
    term(1)->box(box_left, box_top, box_right-box_left, box_bottom-box_top-1, DARK_GREEN, BLACK, true);
    term(1)->fill(box_left+1, box_top+1, box_right, box_top+2, ' ', WHITE, DARK_GREEN);
    term(1)->print_center(box_top+1, std::string(" [") + planet.civs.civs[negotiating_civ].name + std::string("] "), WHITE, GREEN);

    int y = box_top + 3;
    std::stringstream ss;
    ss << "Greetings, Cordex. We are The " << planet.civs.civs[negotiating_civ].name << ".";
    term(1)->print(box_left+1, y, ss.str());
    ++y;

    if (planet.civs.civs[negotiating_civ].cordex_feelings < 0) {
        term(1)->print(box_left+1, y, "You have invaded our lands, harassed our people, and must die.");

        y += 3;
        if (terminal_y == y) {
            term(1)->print(box_left+1, y, "[Perhaps this donation of 50 Mcr will make you feel better]", WHITE, GREEN);
            if (mouse::clicked) {
                game_master_mode = PLAY;
                planet.civs.civs[negotiating_civ].cordex_feelings = 0;
                designations->current_cash -= 50;
            }
        } else {
            term(1)->print(box_left+1, y, "[Perhaps this donation of 50 Mcr will make you feel better]", GREEN, BLACK);
        }
        ++y; ++y;

        if (terminal_y == y) {
            term(1)->print(box_left+1, y, "[I hope you all die in a fire]", WHITE, RED);
            if (mouse::clicked) {
                game_master_mode = PLAY;
                planet.civs.civs[negotiating_civ].cordex_feelings = -10;
            }
        } else {
            term(1)->print(box_left+1, y, "[I hope you all die in a fire]", RED, BLACK);
        }
        ++y; ++y;
    } else if (planet.civs.civs[negotiating_civ].cordex_feelings == 0) {
        term(1)->print(box_left+1, y, "You have occupied our lands, but perhaps we can work something out?");
        y += 3;
        if (terminal_y == y) {
            term(1)->print(box_left+1, y, "[End Negotiation]", WHITE, GREEN);
            if (mouse::clicked) {
                game_master_mode = PLAY;
            }
        } else {
            term(1)->print(box_left+1, y, "[End Negotiation]", GREEN, BLACK);
        }
        ++y; ++y;

        if (terminal_y == y) {
            term(1)->print(box_left+1, y, "[I hope you all die in a fire]", WHITE, RED);
            if (mouse::clicked) {
                game_master_mode = PLAY;
                planet.civs.civs[negotiating_civ].cordex_feelings = -10;
            }
        } else {
            term(1)->print(box_left+1, y, "[I hope you all die in a fire]", RED, BLACK);
        }
    } else {
        term(1)->print(box_left+1, y, "We welcome your presence here.");
        y += 3;

        if (terminal_y == y) {
            term(1)->print(box_left+1, y, "[End negotiation]", WHITE, GREEN);
            if (mouse::clicked) {
                game_master_mode = PLAY;
            }
        } else {
            term(1)->print(box_left+1, y, "[End negotiation]", GREEN, BLACK);
        } 
        ++y; ++y;

        if (terminal_y == y) {
            term(1)->print(box_left+1, y, "[Sorry, time to die!]", WHITE, RED);
        } else {
            term(1)->print(box_left+1, y, "[Sorry, time to die!]", RED, BLACK);
        } 
        ++y; ++y;
    }

}
