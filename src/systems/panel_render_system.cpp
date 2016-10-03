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

std::vector<available_building_t> available_buildings;

const color_t GREEN_BG{0,32,0};

void panel_render_system::update(const double duration_ms) {
	term(2)->clear();

	mouse_damper += duration_ms;

	term(3)->clear();

	render_mode_select(duration_ms);
}

void panel_render_system::configure() {
	system_name = "Panel Render";
	subscribe<refresh_available_buildings_message>([this] (refresh_available_buildings_message &msg) {
		available_buildings = get_available_buildings();
	});
}

void panel_render_system::render_mode_select(const double duration_ms) {
	switch (game_master_mode) {
		//case PLAY : render_play_mode(duration_ms); break;
		case DESIGN : render_design_mode(); break;
		case WORKFLOW : render_work_mode(); break;
		case ROGUE : {
			term(2)->print(28,1,"ESC", YELLOW);
			term(2)->print(32,1,"Return to normal play", WHITE, GREEN_BG);
		} break;
	}
}

inline bool is_mining_designation_valid(const int &x, const int &y, const int &z, const game_mining_mode_t &mode) {
	return true;
}

void panel_render_system::render_design_mode() {
	term(2)->print(32,1, "ESC", YELLOW);
	term(2)->print(36,1, "Resume normal play");

	int mouse_x, mouse_y;
	int font_w, font_h;
	std::tie(mouse_x, mouse_y) = get_mouse_position();
	std::tie(font_w, font_h) = term(1)->get_font_size();
	const int terminal_x = mouse_x / font_w;
	const int terminal_y = mouse_y / font_h;
	const bool clicked = get_mouse_button_state(rltk::button::LEFT);

	if (game_design_mode == DIGGING) {		

		int tt_x = term(3)->term_width - 21;
		term(3)->box(tt_x, 4, 20, 10);
		term(3)->print(tt_x+1, 5, "MINING MODE", WHITE, DARKEST_GREEN);
		term(1)->print(5, 3, "Digging", YELLOW);

		if (is_window_focused()) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) game_mining_mode = DIG;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) game_mining_mode = CHANNEL;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) game_mining_mode = RAMP;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) game_mining_mode = UP;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) game_mining_mode = DOWN;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) game_mining_mode = UPDOWN;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) game_mining_mode = DELETE;
		}

		if (game_mining_mode == DIG) {
			term(3)->print(tt_x+1, 7, "(d) Dig", YELLOW, DARKEST_GREEN);
		} else {
			if (terminal_y == 7 && terminal_x > tt_x+1 && terminal_x < tt_x+8) {
				term(3)->print(tt_x+1,7, "(d) Dig", GREEN, GREEN_BG);
				if (clicked) {
					game_mining_mode = DIG;
					return;
				}
			} else {
				term(3)->print(tt_x+1,7, "(d) Dig", WHITE, GREEN_BG);
			} 
		}
		term(3)->print(tt_x+2, 7, "d", YELLOW); 

		if (game_mining_mode == CHANNEL) { 
			term(3)->print(tt_x+1, 8, "(c) Channel", YELLOW, DARKEST_GREEN);			
		} else { 
			if (terminal_y == 8 && terminal_x > tt_x+1 && terminal_x < tt_x+8) {
				term(3)->print(tt_x+1,8, "(c) Channel", GREEN, GREEN_BG);
				if (clicked) {
					game_mining_mode = CHANNEL;
					return;
				}
			} else {
				term(3)->print(tt_x+1,8, "(c) Channel", WHITE, GREEN_BG);
			} 
		}
		term(3)->print(tt_x+2, 8, "c", YELLOW); 

		if (game_mining_mode == RAMP) { 
			term(3)->print(tt_x+1, 9, "(r) Ramp", YELLOW, DARKEST_GREEN); 
		} else {
			if (terminal_y == 9 && terminal_x > tt_x+1 && terminal_x < tt_x+8) {
				term(3)->print(tt_x+1,9, "(r) Ramp", GREEN, GREEN_BG);
				if (clicked) {
					game_mining_mode = RAMP;
					return;
				}
			} else {
				term(3)->print(tt_x+1,9, "(r) Ramp", WHITE, GREEN_BG);
			} 
		}
		term(3)->print(tt_x+2, 9, "r", YELLOW); 

		if (game_mining_mode == UP) { 
			term(3)->print(tt_x+1, 10, "(u) Up Stairs", YELLOW, DARKEST_GREEN); 
		} else { 
			if (terminal_y == 10 && terminal_x > tt_x+1 && terminal_x < tt_x+8) {
				term(3)->print(tt_x+1,10, "(u) Up Stairs", GREEN, GREEN_BG);
				if (clicked) {
					game_mining_mode = UP;
					return;
				}
			} else {
				term(3)->print(tt_x+1,10, "(u) Up Stairs", WHITE, GREEN_BG);
			} 		
		}
		term(3)->print(tt_x+2, 10, "u", YELLOW); 

		if (game_mining_mode == DOWN) { 
			term(3)->print(tt_x+1, 11, "(j) Down Stairs", YELLOW, DARKEST_GREEN); 
		} else { 
			if (terminal_y == 11 && terminal_x > tt_x+1 && terminal_x < tt_x+8) {
				term(3)->print(tt_x+1,11, "(j) Down Stairs", GREEN, GREEN_BG);
				if (clicked) {
					game_mining_mode = DOWN;
					return;
				}
			} else {
				term(3)->print(tt_x+1,11, "(j) Down Stairs", WHITE, GREEN_BG);
			} 
		}
		term(3)->print(tt_x+2, 11, "j", YELLOW); 

		if (game_mining_mode == UPDOWN) { 
			term(3)->print(tt_x+1, 12, "(i) Up/Down Stairs", YELLOW, DARKEST_GREEN); 
		} else { 
			if (terminal_y == 12 && terminal_x > tt_x+1 && terminal_x < tt_x+8) {
				term(3)->print(tt_x+1,12, "(i) Up/Down Stairs", GREEN, GREEN_BG);
				if (clicked) {
					game_mining_mode = UPDOWN;
					return;
				}
			} else {
				term(3)->print(tt_x+1,12, "(i) Up/Down Stairs", WHITE, GREEN_BG);
			} 
		}
		term(3)->print(tt_x+2, 12, "i", YELLOW); 

		if (game_mining_mode == DELETE) { 
			term(3)->print(tt_x+1 ,13, "(x) Clear", YELLOW, DARKEST_GREEN); 
		} else { 
			if (terminal_y == 13 && terminal_x > tt_x+1 && terminal_x < tt_x+8) {
				term(3)->print(tt_x+1,13, "(x) Clear", WHITE, GREEN_BG);
				if (clicked) {
					game_mining_mode = DELETE;
					return;
				}
			} else {
				term(3)->print(tt_x+1,13, "(x) Clear", WHITE, GREEN_BG);
			} 
		}
		term(3)->print(tt_x+2, 13, "x", YELLOW); 

		if (terminal_x >= 0 && terminal_x < term(1)->term_width && terminal_y >= 3 && terminal_y < term(1)->term_height) {
			if (get_mouse_button_state(rltk::button::LEFT)) {
				const int world_x = std::min(clip_left + terminal_x, REGION_WIDTH);
				const int world_y = std::min(clip_top + terminal_y-2, REGION_HEIGHT);
				const int idx = mapidx(world_x, world_y, camera_position->region_z);
				if (is_mining_designation_valid(world_x, world_y, camera_position->region_z, game_mining_mode)) {
					switch (game_mining_mode) {
						case DIG : designations->mining[idx] = 1; break;
						case CHANNEL : designations->mining[idx] = 2; break;
						case RAMP : designations->mining[idx] = 3; break;
						case UP : designations->mining[idx] = 4; break;
						case DOWN : designations->mining[idx] = 5; break;
						case UPDOWN : designations->mining[idx] = 6; break;
						case DELETE : designations->mining.erase(idx); break;
					}
					emit(map_dirty_message{});
				}
			}
		}

	} else {
		if (terminal_y == 3 && terminal_x > 4 && terminal_x < 13) {
			term(1)->print(5, 3, "Digging", GREEN);
			if (clicked) {
				game_design_mode = DIGGING;
                emit_deferred(map_dirty_message{});
			}
		} else {
			term(1)->print(5, 3, "Digging", WHITE);
			term(1)->print(5,3,"d", YELLOW);
		}
	}

	if (game_design_mode == BUILDING) {
		term(1)->print(13, 3, "Building", YELLOW);

		int tt_x = term(1)->term_width - 30;
		term(1)->box(tt_x, 4, 29, term(1)->term_height-5);
		term(1)->fill(tt_x+1, 5, tt_x+29, term(1)->term_height-2, ' ');
		term(1)->fill(tt_x+1, 5, tt_x+29, 6, ' ', WHITE, DARK_GREEN);
		term(1)->print(tt_x+4, 5, "[Available Buildings]", WHITE, DARK_GREEN);

		int y=7;
		bool rendered_selected = false;
		for (const available_building_t &building : available_buildings) {
			if (build_mode_building && build_mode_building.get().tag == building.tag) {
				term(1)->print(tt_x+1, y, building.get_name(), YELLOW, DARKEST_GREEN);
				rendered_selected = true;
			} else {
				if (terminal_y == y && terminal_x > tt_x && terminal_x) {
					term(1)->print(tt_x+1, y, building.get_name(), GREEN, GREEN_BG);
				} else {
					term(1)->print(tt_x+1, y, building.get_name(), WHITE, GREEN_BG);
				}
			}
			++y;
		}

		if (get_mouse_button_state(rltk::button::LEFT)) {
			
			if (terminal_y > 6 && terminal_y < 7+available_buildings.size() && terminal_x > tt_x+1) {
				const int selected_building = terminal_y - 7;
				if (selected_building > -1 && selected_building < available_buildings.size()) {
					build_mode_building = available_buildings[selected_building];
					rendered_selected = true;
				} else {
					std::cout << "Error: index " << selected_building << " is out of range.\n";
				}
			}
		}
		if (!rendered_selected) build_mode_building.reset();
	} else {
		if (terminal_y == 3 && terminal_x > 12 && terminal_x < 22) {
			term(1)->print(13, 3, "Building", GREEN);
			if (clicked) {
				game_design_mode = BUILDING;
                emit_deferred(refresh_available_buildings_message{});
                emit_deferred(map_dirty_message{});
			}
		} else {
			term(1)->print(13, 3, "Building", WHITE);
			term(1)->print(13, 3, "b", YELLOW);
		}
	}

	if (game_design_mode == CHOPPING) {
		term(1)->print(22, 3, "Tree Cutting", YELLOW);

		int mouse_x, mouse_y;
		int font_w, font_h;
		std::tie(mouse_x, mouse_y) = get_mouse_position();
		std::tie(font_w, font_h) = term(1)->get_font_size();
		const int terminal_x = mouse_x / font_w;
		const int terminal_y = mouse_y / font_h;

		if (terminal_x >= 0 && terminal_x < term(1)->term_width && terminal_y >= 3 && terminal_y < term(1)->term_height) {
			const int world_x = std::min(clip_left + terminal_x, REGION_WIDTH);
			const int world_y = std::min(clip_top + terminal_y-2, REGION_HEIGHT);

			const int idx = mapidx(world_x, world_y, camera_position->region_z);
			const int tree_id = current_region->tree_id[idx];

			if (get_mouse_button_state(rltk::button::LEFT) && tree_id > 0) {
				// Naieve search for the base of the tree; this could be optimized a LOT
				int lowest_z = camera_position->region_z;
				const int stop_z = lowest_z-10;

				position_t tree_pos{world_x, world_y, lowest_z};
				while (lowest_z > stop_z) {
					for (int y=-10; y<10; ++y) {
						for (int x=-10; x<10; ++x) {
							const int tree_idx = mapidx(world_x + x, world_y + y, lowest_z);
							if (current_region->tree_id[tree_idx] == tree_id) {
								tree_pos.x = world_x+x;
								tree_pos.y = world_y+y;
								tree_pos.z = lowest_z;
							}
						}
					}
					--lowest_z;
				}

				designations->chopping[tree_id] = tree_pos;
				emit(map_dirty_message{});
			} else if (get_mouse_button_state(rltk::button::RIGHT) && tree_id > 0) {
				designations->chopping.erase(tree_id);
				emit(map_dirty_message{});
			}
		}			
	} else {
		if (terminal_y == 3 && terminal_x > 22 && terminal_x < 32) {
			term(1)->print(22, 3, "Tree Cutting", GREEN);
			if (clicked) {
				game_design_mode = CHOPPING;
				emit_deferred(map_dirty_message{});
			}
		} else {
			term(1)->print(22, 3, "Tree Cutting", WHITE);
			term(1)->print(22, 3, "t", YELLOW);
		}
	}

	if (game_design_mode == GUARDPOINTS) {
		term(1)->print(35, 3, "Guard Posts", WHITE, GREEN);

		int mouse_x, mouse_y;
		int font_w, font_h;
		std::tie(mouse_x, mouse_y) = get_mouse_position();
		std::tie(font_w, font_h) = term(1)->get_font_size();
		const int terminal_x = mouse_x / font_w;
		const int terminal_y = mouse_y / font_h;

		if (terminal_x >= 0 && terminal_x < term(1)->term_width && terminal_y >= 3 && terminal_y < term(1)->term_height) {
			const int world_x = std::min(clip_left + terminal_x, REGION_WIDTH);
			const int world_y = std::min(clip_top + terminal_y-2, REGION_HEIGHT);

			const int idx = mapidx(world_x, world_y, camera_position->region_z);
			if (current_region->tile_flags[idx].test(CAN_STAND_HERE)) {
				if (get_mouse_button_state(rltk::button::LEFT)) {
					bool found = false;
					for (const auto &g : designations->guard_points) {
						if (mapidx(g.second) == idx) found = true;
					}
					if (!found) designations->guard_points.push_back(std::make_pair(false, position_t{world_x, world_y, camera_position->region_z}));
				} else if (get_mouse_button_state(rltk::button::RIGHT)) {
					designations->guard_points.erase(std::remove_if(
							designations->guard_points.begin(),
							designations->guard_points.end(),
							[&idx] (std::pair<bool,position_t> p) { return idx == mapidx(p.second); }
						),
						designations->guard_points.end());
				}
			}
		}
	} else {
		if (terminal_y == 3 && terminal_x > 35 && terminal_x < 49) {
			term(1)->print(35, 3, "Guard Posts", GREEN);
			if (clicked) {
				game_design_mode = GUARDPOINTS;
                emit_deferred(map_dirty_message{});
			}
		} else {
			term(1)->print(35, 3, "Guard Posts", WHITE);
			term(1)->print(35, 3, "g", YELLOW);
		}
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
