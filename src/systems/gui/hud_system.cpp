#include "hud_system.hpp"
#include "gui_system.hpp"
#include "../../main/game_globals.hpp"
#include "../../messages/messages.hpp"
#include <sstream>

constexpr int HUD_TERM = 3;

void hud_system::configure() {
    system_name = "HUD System";
}

void hud_system::update(const double ms) {
    // Add date/time
    add_gui_element<map_static_text>(0, 0, calendar->get_date_time());

    // Pause mode
    if (pause_mode) {
        add_gui_element<map_static_text>(term(HUD_TERM)->term_width - 8, 0, "*PAUSED*", rltk::colors::WHITE, rltk::colors::BLUE);
	}

    // Display power meter
    add_gui_element<gui_power_meter>(designations->current_power, designations->total_capacity, designations->alert_color);

    // Display cash
    std::stringstream cash_ss;
	cash_ss << "Cash: " << designations->current_cash << " Mcr";
	const std::string cash_str = cash_ss.str();
	add_gui_element<map_static_text>( term(HUD_TERM)->term_width - cash_str.size(), 0, cash_str, rltk::colors::YELLOW );

    // Setup the main menu
    add_gui_element<gui_menu_bar>(std::vector<std::string>{"Play", "Design", "Units", "Workflow", "Civs", "Standing Orders"}, 0, 1, [] (int key) {
        switch(key) {
            case 0 : {
                game_master_mode = PLAY;
                emit_deferred(map_dirty_message{});
                emit_deferred(recalculate_mining_message{});
            } break;
            case 1 : {
                game_master_mode = DESIGN;
                pause_mode = PAUSED;
                emit_deferred(map_dirty_message{});
            } break;
            case 2 : {
                game_master_mode = UNITS;
                pause_mode = PAUSED;
                emit_deferred(map_dirty_message{});
            } break;
            case 3 : {
                game_master_mode = WORKFLOW;
                pause_mode = PAUSED;
                emit_deferred(map_dirty_message{});
            } break;
            case 4 : {
                game_master_mode = CIVS;
                pause_mode = PAUSED;
                emit_deferred(map_dirty_message{});
            } break;
            case 5 : {
                game_master_mode = STANDING_ORDERS;
                pause_mode = PAUSED;
                emit_deferred(map_dirty_message{});
            }
        }
    });
}
