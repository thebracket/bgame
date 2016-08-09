#include "mode_units_system.hpp"
#include "../game_globals.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../messages/recalculate_mining_message.hpp"
#include "../utils/string_utils.hpp"
#include <rltk.hpp>

using namespace rltk;
using namespace rltk::colors;

void mode_units_system::configure() {
    subscribe_mbox<key_pressed_t>();
}

void mode_units_system::update(const double ms) {
    if (game_master_mode != UNITS) return;

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
    term(1)->print_center(box_top+1, " [Active Units] ", WHITE, GREEN);

    // Tabs
    if (tab == 0) {
        term(1)->box(box_left+1, box_top+2, 9, 2, GREEN, BLACK, false);
        term(1)->fill(box_left+2, box_top+3, box_left+11, box_top+3, ' ', DARK_GREEN, BLACK);
        term(1)->print(box_left+2, box_top+3, "Settlers", WHITE, BLACK);

        int y = box_top + 6;
        each<settler_ai_t, name_t, game_stats_t>([this, &y, &terminal_x, &terminal_y] (entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats) {
            color_t background = BLACK;

            if (terminal_y == y && terminal_x > 21) {
                background = DARK_GREEN;
                if (get_mouse_button_state(rltk::button::LEFT)) {
                    selected_settler = e.id;
                    game_master_mode = SETTLER;
                }
            }

            if (terminal_y == y && terminal_x > box_left+3 && terminal_x < box_left+8) {
                term(1)->print(box_left+3, y, " GO ", WHITE, DARK_GREEN);
                if (get_mouse_button_state(rltk::button::LEFT)) {
                    // Move the camera and close
                    game_master_mode = PLAY;
                    emit(map_dirty_message{});
                    emit(recalculate_mining_message{});
                    position_t * pos = e.component<position_t>();
                    camera_position->region_x = pos->x;
                    camera_position->region_y = pos->y;
                    camera_position->region_z = pos->z;
                }
            } else {
                term(1)->print(box_left+3, y, " GO ", WHITE, DARKEST_GREEN);
            }
            if (terminal_y == y && terminal_x > box_left+8 && terminal_x < box_left+19) {
                term(1)->print(box_left+8, y, " CONTROL ", WHITE, DARK_GREEN);
                if (get_mouse_button_state(rltk::button::LEFT)) {
                    // Switch to rogue mode and close
                    game_master_mode = ROGUE;
                    selected_settler = e.id;
                    emit(map_dirty_message{});
                    emit(recalculate_mining_message{});
                    position_t * pos = e.component<position_t>();
                    camera_position->region_x = pos->x;
                    camera_position->region_y = pos->y;
                    camera_position->region_z = pos->z;
                    entity(e.id)->component<settler_ai>()->job_type_major = JOB_IDLE;
                }
            } else {
                term(1)->print(box_left+8, y, " CONTROL ", WHITE, DARKEST_GREEN);
            }
            
            term(1)->print(22, y, max_width_str(name.first_name + std::string(" ") + name.last_name, 20), WHITE, background);
            term(1)->print(42, y, max_width_str(stats.profession_tag, 14), WHITE, background);
            term(1)->print(57, y, max_width_str(ai.job_status, 29), WHITE, background);

            ++y;
        });
    } else {
        term(1)->box(box_left+1, box_top+2, 9, 2, DARK_GREEN, BLACK, false);
        term(1)->fill(box_left+2, box_top+3, box_left+11, box_top+3, ' ', BLACK, BLACK);
        term(1)->print(box_left+2, box_top+3, "Settlers", DARK_GREEN, BLACK);
    }

    if (tab == 1) {
        term(1)->box(box_left+11, box_top+2, 10, 2, GREEN, BLACK, false);
        term(1)->fill(box_left+12, box_top+3, box_left+26, box_top+3, ' ', DARK_GREEN, BLACK);
        term(1)->print(box_left+12, box_top+3, "Creatures", WHITE, BLACK);
    } else {
        term(1)->box(box_left+11, box_top+2, 10, 2, DARK_GREEN, BLACK, false);
        term(1)->fill(box_left+12, box_top+3, box_left+26, box_top+3, ' ', BLACK, BLACK);
        term(1)->print(box_left+12, box_top+3, "Creatures", DARK_GREEN, BLACK);
    }

    /*
    int y = 5;
	term(1)->box(3, 2, 70, 40, WHITE, BLACK, true);
	for (int i=3; i<42; ++i) term(1)->print(4, i, "                                                                     ");

	term(1)->print(5,4,"Settler Name        Profession     Current Status", YELLOW, BLACK);

	int mouse_x, mouse_y;
	std::tie(mouse_x, mouse_y) = get_mouse_position();
	int terminal_x = mouse_x/8;
	int terminal_y = (mouse_y/8);

	each<settler_ai_t, name_t, game_stats_t>([this, &y, &terminal_x, &terminal_y] (entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats) {
		color_t background = BLACK;
		
		if (terminal_y == y && terminal_x > 4 && terminal_x < 70) {
			background = BLUE;

			if (get_mouse_button_state(rltk::button::LEFT)) {
				selected_settler = e.id;
				game_master_mode = SETTLER;
			}
		}

		term(1)->print(5, y, max_width_str(name.first_name + std::string(" ") + name.last_name, 19), WHITE, background);
		term(1)->print(25, y, max_width_str(stats.profession_tag, 14), WHITE, background);
		term(1)->print(40, y, max_width_str(ai.job_status, 29), WHITE, background);
		++y;
	});*/
}
