#include "panel_render_system.hpp"
#include "../main/game_globals.hpp"
#include "../raws/raws.hpp"
#include "../components/components.hpp"
#include "../messages/messages.hpp"
#include "mining_system.hpp"
#include "inventory_system.hpp"
#include "camera_system.hpp"
#include "../utils/string_utils.hpp"
#include <sstream>
#include <iomanip>
#include <map>

using namespace rltk;
using namespace rltk::colors;

const color_t GREEN_BG{0,32,0};

void panel_render_system::update(const double duration_ms) {
	term(2)->clear();

	mouse_damper += duration_ms;

	term(3)->clear();

	render_mode_select(duration_ms);
}

void panel_render_system::configure() {
	system_name = "Panel Render";	
}

void panel_render_system::render_mode_select(const double duration_ms) {
	switch (game_master_mode) {
		//case DESIGN : render_design_mode(); break;
		case WORKFLOW : render_work_mode(); break;
		case ROGUE : {
			term(2)->print(28,1,"ESC", YELLOW);
			term(2)->print(32,1,"Return to normal play", WHITE, GREEN_BG);
		} break;
	}
}

void panel_render_system::render_work_mode() {
	term(1)->box(1, 2, 73, 60, WHITE, BLACK, true);
	for (int i=3; i<60; ++i) term(1)->print(2, i, "                                                                        ");

	// List queued jobs
	int y = 3;
	for (const std::pair<uint8_t, std::string> &order : designations->build_orders) {
		std::stringstream ss;
		auto finder = reaction_defs.find(order.second);
		if (finder != reaction_defs.end()) {
			ss << std::setw(4) << +order.first << "  " << finder->second.name;
			term(1)->print(3, y, ss.str());
			++y;
		}
	}

	// List available tasks with "build it" button
	y = 3;
	auto available_reactions = get_available_reactions();
	for (const auto &reaction : available_reactions) {
		std::stringstream ss;
		ss << "[+] " << reaction.second;
		term(1)->print(40, y, ss.str());
		++y;
	}

	// Check for clicks to build
	if (get_mouse_button_state(rltk::button::LEFT) && mouse_damper > 20.0) {
		mouse_damper = 0.0;		
		int mouse_x, mouse_y;
		std::tie(mouse_x, mouse_y) = get_mouse_position();
		int terminal_x = mouse_x/8;
		int terminal_y = (mouse_y/8);

		if (terminal_x > 40 && terminal_y > 2 && terminal_y << y && terminal_x < 45) {
			const int selected_item = terminal_y - 3;
			const std::string tag = available_reactions[selected_item].first;

			bool found = false;
			for (auto &order : designations->build_orders) {
				if (order.second == tag) {
					++order.first;
					found = true;
				}
			}
			if (!found) designations->build_orders.push_back(std::make_pair(1, tag));
		}
	}

}
