#include "mode_civs_system.hpp"
#include "../main/game_globals.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../messages/recalculate_mining_message.hpp"
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

    std::queue<key_pressed_t> * messages = mbox<key_pressed_t>();
    while (!messages->empty()) {
		key_pressed_t e = messages->front();
		messages->pop();

        if (e.event.key.code == sf::Keyboard::Escape) {
            game_master_mode = PLAY;
            emit(map_dirty_message{});
            emit(recalculate_mining_message{});
        }
    }

    if (game_master_mode == CIVS) {
        render_civ_list();
    } else {
        render_civ_negotiate();
    }
}

void render_civ_list() {
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
    term(1)->print_center(box_top+1, " [Active Civilizations] ", WHITE, GREEN);

    int y = box_top + 3;
    std::size_t civ_id = 0;
    for (civ_t &civ : planet.civs.civs) {
        if (civ.met_cordex) {
            if (civ.cordex_feelings < 0) term(1)->print(box_left+2, y,  "  WAR   ", WHITE, RED);
            if (civ.cordex_feelings == 0) term(1)->print(box_left+2, y, "NEUTRAL ", BLACK, CYAN);
            if (civ.cordex_feelings > 0) term(1)->print(box_left+2, y,  "FRIENDLY", WHITE, GREEN);

            if (terminal_y == y && terminal_x > box_left + 10 && terminal_x < box_left + 15) {
                term(1)->print(box_left+10, y, "Negotiate", WHITE, DARK_GREEN);
                if (get_mouse_button_state(rltk::button::LEFT)) {
                    game_master_mode = CIV_NEGOTIATE;
                    negotiating_civ = civ_id;
                }
            } else {
                term(1)->print(box_left+10, y, "Negotiate", GREEN, BLACK);
            }

            term(1)->print(box_left+20, y, civ.name);
            ++y;
        }
        ++civ_id;
    }

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
            if (get_mouse_button_state(rltk::button::LEFT)) {
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
            if (get_mouse_button_state(rltk::button::LEFT)) {
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
            if (get_mouse_button_state(rltk::button::LEFT)) {
                game_master_mode = PLAY;
            }
        } else {
            term(1)->print(box_left+1, y, "[End Negotiation]", GREEN, BLACK);
        }
        ++y; ++y;
    } else {
        term(1)->print(box_left+1, y, "We welcome your presence here.");
        y += 3;

        if (terminal_y == y) {
            term(1)->print(box_left+1, y, "[End negotiation]", WHITE, GREEN);
            if (get_mouse_button_state(rltk::button::LEFT)) {
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
