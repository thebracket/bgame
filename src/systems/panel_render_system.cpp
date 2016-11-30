#include "panel_render_system.hpp"
#include "../main/game_globals.hpp"
#include "../raws/raws.hpp"
#include "../raws/reactions.hpp"
#include "../messages/messages.hpp"
#include "mining_system.hpp"
#include "inventory_system.hpp"
#include "camera_system.hpp"
#include "../utils/string_utils.hpp"
#include "mouse_input_system.hpp"
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
		case WORKFLOW : render_work_mode(); break;
		case ROGUE : {
			term(2)->print(28,1,"ESC", YELLOW);
			term(2)->print(32,1,"Return to normal play", WHITE, GREEN_BG);
		} break;
	}
}

void panel_render_system::render_work_mode() {
	term(3)->box(1, 2, 73, 40, WHITE, BLACK, true);
	for (int i=3; i<40; ++i) term(1)->print(3, i, "                                                                        ");

	// List queued jobs
	int y = 3;
	for (const std::pair<uint8_t, std::string> &order : designations->build_orders) {
		std::stringstream ss;
		auto finder = reaction_defs.find(order.second);
		if (finder != reaction_defs.end()) {
			ss << std::setw(4) << +order.first << "  " << finder->second.name;
			term(3)->print(3, y, ss.str());
			++y;
		}
	}

	// List available tasks with "build it" button
	y = 3;
	auto available_reactions = get_available_reactions();
	for (const auto &reaction : available_reactions) {
		std::stringstream ss;
		ss << "[+] " << reaction.second;
		term(3)->print(40, y, ss.str());
		++y;
	}

	// Check for clicks to build
	if (mouse::clicked) {

		if (mouse::term3x > 40 && mouse::term3y > 2 && mouse::term3y << y && mouse::term3x < 45) {
			const int selected_item = mouse::term3y - 3;
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
