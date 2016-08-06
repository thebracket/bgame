#include "panel_render_system.hpp"
#include "../game_globals.hpp"
#include "../raws/raws.hpp"
#include "../components/components.hpp"
#include "../messages/messages.hpp"
#include "mining_system.hpp"
#include "inventory_system.hpp"
#include "camera_system.hpp"
#include <sstream>
#include <iomanip>
#include <map>

using namespace rltk;
using namespace rltk::colors;

std::vector<available_building_t> available_buildings;

const color_t GREEN_BG{0,32,0};

void panel_render_system::render_header() {
	term(2)->clear();
	term(1)->print(0,0,calendar->get_date_time(), rltk::colors::WHITE);
	if (pause_mode) term(1)->print(term(2)->term_width - 8, 1, "*PAUSED*", rltk::colors::WHITE, rltk::colors::BLUE);
	float power_pct = (float)designations->current_power / (float) designations->total_capacity;
    if (power_pct < 0.5) {
        designations->alert_color = lerp(rltk::colors::RED, rltk::colors::ORANGE, power_pct*2.0F);
    } else {
        designations->alert_color = lerp(rltk::colors::ORANGE, rltk::colors::GREEN, (power_pct-0.5F)*2.0F);
    }
    std::stringstream power_ss;
    power_ss << " Power: " << designations->current_power << "/" << designations->total_capacity << " ";

	const int power_width = term(2)->term_width - 30;
	const int power_pips_filled = (float)power_width * power_pct;
	const float pct_per_pip = 1.0 / (float)designations->total_capacity;
	for (int x=0; x<power_width; ++x) {
		const color_t pip_color = lerp(rltk::colors::GREY, designations->alert_color, pct_per_pip * (float)x); 
		if (x <= power_pips_filled) {
			term(1)->set_char(x+15, 0, vchar{176, pip_color, rltk::colors::BLACK});
		} else {
			term(1)->set_char(x+15, 0, vchar{7, pip_color, rltk::colors::BLACK});			
		}
	}
	const std::string power_str = power_ss.str();
	const int power_x = ((term(1)->term_width-30)/2) - (power_str.size() / 2);
	term(1)->print( power_x, 0, power_str, designations->alert_color );

	std::stringstream cash_ss;
	cash_ss << "Cash: " << designations->current_cash << " Mcr";
	const std::string cash_str = cash_ss.str();
	term(1)->print( term(2)->term_width - cash_str.size(), 0, cash_str, rltk::colors::YELLOW );
}

void panel_render_system::update(const double duration_ms) {
	render_header();

	mouse_damper += duration_ms;

	term(3)->clear();
	//term(3)->box(DARKEST_GREEN);

	render_mode_select(duration_ms);

	if (game_master_mode == PLAY) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			game_master_mode = DESIGN;
			pause_mode = PAUSED;
			emit(map_dirty_message{});
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
			game_master_mode = UNITS;
			pause_mode = PAUSED;
			emit(map_dirty_message{});
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			game_master_mode = WORKFLOW;
			pause_mode = PAUSED;
			emit(map_dirty_message{});
		}
	} else if (game_master_mode == DESIGN) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			game_master_mode = PLAY;
			emit(map_dirty_message{});
			emit(recalculate_mining_message{});
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			game_design_mode = DIGGING;
			emit(map_dirty_message{});
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
			game_design_mode = BUILDING;
			available_buildings = get_available_buildings();
			emit(map_dirty_message{});
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
			game_design_mode = CHOPPING;
			emit(map_dirty_message{});
		}
	} else if (game_master_mode == UNITS) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			game_master_mode = PLAY;
			emit(map_dirty_message{});
			emit(recalculate_mining_message{});
		}
	} else if (game_master_mode == SETTLER) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			game_master_mode = PLAY;
			emit(map_dirty_message{});
			emit(recalculate_mining_message{});
		}
	} else if (game_master_mode == WORKFLOW) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			game_master_mode = PLAY;
			emit(map_dirty_message{});
			emit(recalculate_mining_message{});
		}
	}
}

void panel_render_system::configure() {
	system_name = "Panel Render";
	subscribe<refresh_available_buildings_message>([this] (refresh_available_buildings_message &msg) {
		available_buildings = get_available_buildings();
	});
}

void panel_render_system::render_mode_select(const double duration_ms) {
	// Mode switch controls
	if (game_master_mode == PLAY) {
		term(2)->print(1,1,"Play", YELLOW, BLUE);
		render_play_mode(duration_ms);
	} else {
		term(2)->print(1,1,"Play", WHITE, BLACK);
	}

	if (game_master_mode == DESIGN) {
		term(2)->print(6,1,"Design", YELLOW, BLUE);
		render_design_mode();
	} else {
		term(2)->print(6,1,"D", YELLOW, BLACK);
		term(2)->print(7,1,"esign", WHITE, BLACK);
	}

	if (game_master_mode == UNITS) {
		term(2)->print(13,1,"Units", YELLOW, BLUE);
		render_units_mode();
	} else {
		term(2)->print(13,1,"U", YELLOW, BLACK);
		term(2)->print(14,1,"nits", WHITE, BLACK);
	}

	if (game_master_mode == WORKFLOW) {
		term(2)->print(19,1,"Workflow", YELLOW, BLUE);
		render_work_mode();
	} else {
		term(2)->print(19,1,"W", YELLOW, BLACK);
		term(2)->print(20,1,"orkflow", WHITE, BLACK);
	}

	if (game_master_mode == SETTLER) {
		render_settler_mode();
	}
}

void panel_render_system::render_play_mode(const double duration_ms) {
	// Controls Help
	if (pause_mode == PAUSED) {
		term(2)->print(28,1,"SPACE", YELLOW);
		term(2)->print(33,1," Unpause", WHITE, GREEN_BG);
	} else {
		term(2)->print(28,1,"SPACE", YELLOW);
		term(2)->print(33,1," Pause", WHITE, GREEN_BG);
	}
	term(2)->print(42,1,".",YELLOW);
	term(2)->print(43,1," One Step", WHITE, GREEN_BG);

	// Mouse tips
	int mouse_x, mouse_y;
	int font_w, font_h;
	std::tie(mouse_x, mouse_y) = get_mouse_position();
	std::tie(font_w, font_h) = term(1)->get_font_size();

	// Since we're using an 8x8, it's just a matter of dividing by 8 to find the terminal-character
	// coordinates. There will be a helper function for this once we get into retained GUIs.
	const int terminal_x = mouse_x / font_w;
	const int terminal_y = mouse_y / font_h;
	const int world_x = std::min(clip_left + terminal_x, REGION_WIDTH);
	const int world_y = std::min(clip_top + terminal_y-2, REGION_HEIGHT);
	const int tile_idx = mapidx( world_x, world_y, camera_position->region_z );
	bool tooltip = false;
	
	if (terminal_x == last_mouse_x && terminal_y == last_mouse_y && world_y > 0) {
		mouse_dwell_time += duration_ms;
		if (mouse_dwell_time > 200.0 && tile_idx !=0 && current_region->revealed[tile_idx] ) tooltip = true;
	} else {
		last_mouse_x = terminal_x;
		last_mouse_y = terminal_y;
		mouse_dwell_time = 0.0;
	}

	if (tooltip) {
		std::vector<std::string> lines;

		{
			std::stringstream ss;
			if (current_region->tile_flags[tile_idx].test(CAN_GO_DOWN)) ss << "Down-";
			if (current_region->tile_flags[tile_idx].test(CAN_GO_UP)) ss << "Up-";
			if (current_region->tile_flags[tile_idx].test(CAN_GO_NORTH)) ss << "North-";
			if (current_region->tile_flags[tile_idx].test(CAN_GO_EAST)) ss << "East-";
			if (current_region->tile_flags[tile_idx].test(CAN_GO_WEST)) ss << "West-";
			if (current_region->tile_flags[tile_idx].test(CAN_GO_SOUTH)) ss << "South-";
			if (current_region->tile_flags[tile_idx].test(CAN_STAND_HERE)) ss << "Stand";
			lines.push_back(ss.str());
		}
		if (current_region->water_level[tile_idx]>0) lines.push_back(std::string("Water level: " + std::to_string(current_region->water_level[tile_idx])));

		{ // Base tile type
			std::stringstream ss;
			switch (current_region->tile_type[tile_idx]) {
				case tile_type::SEMI_MOLTEN_ROCK : ss << "Magma"; break;
				case tile_type::SOLID : ss << "Solid Rock (" << material_defs[current_region->tile_material[tile_idx]].name << ")"; break;
				case tile_type::OPEN_SPACE : ss << "Open Space"; break;
				case tile_type::WALL : ss << "Wall (" << material_defs[current_region->tile_material[tile_idx]].name << ")"; break;
				case tile_type::RAMP : ss << "Ramp (" << material_defs[current_region->tile_material[tile_idx]].name << ")"; break;
				case tile_type::STAIRS_UP : ss << "Up Stairs (" << material_defs[current_region->tile_material[tile_idx]].name << ")"; break;
				case tile_type::STAIRS_DOWN : ss << "Down Stairs (" << material_defs[current_region->tile_material[tile_idx]].name << ")"; break;
				case tile_type::STAIRS_UPDOWN : ss << "Spiral Stairs (" << material_defs[current_region->tile_material[tile_idx]].name << ")"; break;
				case tile_type::FLOOR : ss << "Floor (" << material_defs[current_region->tile_material[tile_idx]].name << ")"; break;
				case tile_type::TREE_TRUNK : ss << "Tree Trunk"; break;
				case tile_type::TREE_LEAF : ss << "Tree Foliage"; break;
				default : ss << "Unknown!";
			}
			lines.push_back(ss.str());
		}
		if (current_region->tile_type[tile_idx] == tile_type::FLOOR && !current_region->tile_flags[tile_idx].test(CONSTRUCTION)) {
			if (current_region->tile_vegetation_type[tile_idx] > 0) {
				lines.push_back(plant_defs[current_region->tile_vegetation_type[tile_idx]].name);
			}
		}

		// Named entities in the location
		each<name_t, position_t>([&lines,&world_x, &world_y] (entity_t &entity, name_t &name, position_t &pos) {
			if (pos.x == world_x && pos.y == world_y && pos.z == camera_position->region_z) {
				lines.push_back(name.first_name + std::string(" ") + name.last_name);
			}
		});
		// Items on the ground
		std::map<std::string, int> items;
		each<item_t, position_t>([&world_x, &world_y, &items] (entity_t &entity, item_t &item, position_t &pos) {
			if (pos.x == world_x && pos.y == world_y && pos.z == camera_position->region_z) {
				auto finder = items.find(item.item_name);
				if (finder == items.end()) {
					items[item.item_name] = 1;
				} else {
					++finder->second;
				}
			}
		});
		for (auto it=items.begin(); it!=items.end(); ++it) {
			std::string n = std::to_string(it->second);
			lines.push_back(n + std::string("x ") + it->first);
		}

		// Storage lockers and similar
		items.clear();
		each<construct_container_t, position_t>([&world_x, &world_y, &items] (entity_t &storage_entity, construct_container_t &container, position_t &pos) {
			if (pos.x == world_x && pos.y == world_y && pos.z == camera_position->region_z) {
				// It is a container and it is here - look inside!
				each<item_t, item_stored_t>([&items,&world_x, &world_y, &storage_entity] (entity_t &entity, item_t &item, item_stored_t &stored) {
					if (stored.stored_in == storage_entity.id) {
						auto finder = items.find(item.item_name);
						if (finder == items.end()) {
							items[item.item_name] = 1;
						} else {
							++finder->second;
						}
					}
				});
			}
		});
		for (auto it=items.begin(); it!=items.end(); ++it) {
			std::string n = std::to_string(it->second);
			lines.push_back(n + std::string("x ") + it->first);
		}

		// Buildings
		each<building_t, position_t>([&lines,&world_x, &world_y] (entity_t &building_entity, building_t &building, position_t &pos) {
			if (pos.x == world_x && pos.y == world_y && pos.z == camera_position->region_z) {
				// It's building and we can see it
				auto finder = building_defs.find(building.tag);
				std::string building_name = "Unknown Building";
				if (finder != building_defs.end()) {
					if (building.complete) {
						building_name = finder->second.name;
					} else {
						building_name = std::string("...") + finder->second.name;
					}
				}
				lines.push_back(building_name);
			}
		});

		int longest = 0;
		for (const std::string &s : lines) {
			if (s.size() > longest) longest = s.size();
		}

		// TODO - dynamic placement
		float revealed_pct = mouse_dwell_time - 200.0;
		revealed_pct /= game_config.tooltip_speed;
		if (revealed_pct > 1.0) revealed_pct = 1.0;
		if (!game_config.tooltip_fadein) revealed_pct = 1.0;

		if (revealed_pct < 1.0) {
			for (std::string &s : lines) {
				int n_garbled = s.size() - ((float)s.size() * (revealed_pct/2.0));
				for (int i=0; i<n_garbled; ++i) s[i] = static_cast<uint8_t>(rng.roll_dice(1,255));
			}
		}

		auto color = lerp(BLACK, LIGHT_GREEN, revealed_pct);
		int tt_x = terminal_x+2;
		int tt_y = terminal_y;
		term(1)->set_char(terminal_x+1, terminal_y, vchar{27, color, BLACK});
		term(1)->box(tt_x, tt_y, longest+1, lines.size()+1, color);
		++tt_y;
		for (const std::string &s : lines) {
			term(1)->print(tt_x+1, tt_y, s, color);
			++tt_y;
		}
	}
}

inline bool is_mining_designation_valid(const int &x, const int &y, const int &z, const game_mining_mode_t &mode) {
	return true;
}

void panel_render_system::render_design_mode() {
	term(2)->print(32,1, "ESC", YELLOW);
	term(2)->print(36,1, "Resume normal play");

	if (game_design_mode == DIGGING) {		

		int tt_x = term(1)->term_width - 21;
		term(1)->box(tt_x, 4, 20, 10);
		term(1)->print(tt_x+1, 5, "MINING MODE", WHITE, DARKEST_GREEN);
		term(1)->print(5, 3, "Digging", YELLOW);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) game_mining_mode = DIG;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) game_mining_mode = CHANNEL;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) game_mining_mode = RAMP;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) game_mining_mode = UP;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) game_mining_mode = DOWN;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) game_mining_mode = UPDOWN;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) game_mining_mode = DELETE;

		if (game_mining_mode == DIG) {
			term(1)->print(tt_x+1, 7, "(d) Dig", YELLOW, DARKEST_GREEN);
		} else { 
			term(1)->print(tt_x+1,7, "(d) Dig", WHITE, GREEN_BG); 
		}
		term(1)->print(tt_x+2, 7, "d", YELLOW); 

		if (game_mining_mode == CHANNEL) { 
			term(1)->print(tt_x+1, 8, "(c) Channel", YELLOW, DARKEST_GREEN);			
		} else { 
			term(1)->print(tt_x+1,8, "(c) Channel", WHITE, GREEN_BG); 
		}
		term(1)->print(tt_x+2, 8, "c", YELLOW); 

		if (game_mining_mode == RAMP) { 
			term(1)->print(tt_x+1, 9, "(r) Ramp", YELLOW, DARKEST_GREEN); 
		} else { 
			term(1)->print(tt_x+1,9, "(r) Ramp", WHITE, GREEN_BG); 
		}
		term(1)->print(tt_x+2, 9, "r", YELLOW); 

		if (game_mining_mode == UP) { 
			term(1)->print(tt_x+1, 10, "(u) Up Stairs", YELLOW, DARKEST_GREEN); 
		} else { 
			term(1)->print(tt_x+1,10, "(u) Up Stairs", WHITE, GREEN_BG); 		
		}
		term(1)->print(tt_x+2, 10, "u", YELLOW); 

		if (game_mining_mode == DOWN) { 
			term(1)->print(tt_x+1, 11, "(j) Down Stairs", YELLOW, DARKEST_GREEN); 
		} else { 
			term(1)->print(tt_x+1,11, "(j) Down Stairs", WHITE, GREEN_BG); 
		}
		term(1)->print(tt_x+2, 11, "j", YELLOW); 

		if (game_mining_mode == UPDOWN) { 
			term(1)->print(tt_x+1, 12, "(i) Up/Down Stairs", YELLOW, DARKEST_GREEN); 
		} else { 
			term(1)->print(tt_x+1,12, "(i) Up/Down Stairs", WHITE, GREEN_BG); 
		}
		term(1)->print(tt_x+2, 12, "i", YELLOW); 

		if (game_mining_mode == DELETE) { 
			term(1)->print(tt_x+1 ,13, "(x) Clear", YELLOW, DARKEST_GREEN); 
		} else { 
			term(1)->print(tt_x+1,13, "(x) Clear", WHITE, GREEN_BG); 
		}
		term(1)->print(tt_x+2, 13, "x", YELLOW); 

		int mouse_x, mouse_y;
		int font_w, font_h;
		std::tie(mouse_x, mouse_y) = get_mouse_position();
		std::tie(font_w, font_h) = term(1)->get_font_size();
		const int terminal_x = mouse_x / font_w;
		const int terminal_y = mouse_y / font_h;

		if (terminal_x >= 0 && terminal_x < term(1)->term_width && terminal_y >= 0 && terminal_y < term(1)->term_height) {
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
		term(1)->print(5, 3, "Digging", WHITE);
		term(1)->print(5,3,"d", YELLOW);
	}

	if (game_design_mode == BUILDING) {
		term(1)->print(13, 3, "Building", YELLOW);

		int tt_x = term(1)->term_width - 30;
		term(1)->box(tt_x, 4, 29, term(1)->term_height-5);
		term(1)->print(tt_x+1, 5, "Available Buildings", WHITE, DARKEST_GREEN);

		int y=7;
		for (const available_building_t &building : available_buildings) {
			if (build_mode_building && build_mode_building.get().tag == building.tag) {
				term(1)->print(tt_x+1, y, building.name, YELLOW, DARKEST_GREEN);
			} else {
				term(1)->print(tt_x+1, y, building.name, WHITE, GREEN_BG);
			}
			++y;
		}

		if (get_mouse_button_state(rltk::button::LEFT)) {
			int mouse_x, mouse_y;
			int font_w, font_h;
			std::tie(mouse_x, mouse_y) = get_mouse_position();
			std::tie(font_w, font_h) = term(1)->get_font_size();

			int terminal_x = mouse_x / font_w;
			int terminal_y = mouse_y / font_h;
			
			if (terminal_y > 6 && terminal_y < 7+available_buildings.size() && terminal_x > tt_x+1) {
				const int selected_building = terminal_y - 7;
				if (selected_building > -1 && selected_building < available_buildings.size()) {
					build_mode_building = available_buildings[selected_building];
				} else {
					std::cout << "Error: index " << selected_building << " is out of range.\n";
				}
			}
		}
	} else {
		term(1)->print(13, 3, "Building", WHITE);
		term(1)->print(13, 3, "b", YELLOW);
	}

	if (game_design_mode == CHOPPING) {
		term(1)->print(22, 3, "Tree Cutting", YELLOW);

		int mouse_x, mouse_y;
		int font_w, font_h;
		std::tie(mouse_x, mouse_y) = get_mouse_position();
		std::tie(font_w, font_h) = term(1)->get_font_size();
		const int terminal_x = mouse_x / font_w;
		const int terminal_y = mouse_y / font_h;

		if (terminal_x >= 0 && terminal_x < term(1)->term_width && terminal_y >= 0 && terminal_y < term(1)->term_height) {
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
		term(1)->print(22, 3, "Tree Cutting", WHITE);
		term(1)->print(22, 3, "t", YELLOW);
	}
}

std::string max_width_str(const std::string original, const int width) {
	if (original.size() <= width) return original;
	return original.substr(0,width);
}

void panel_render_system::render_units_mode() {
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
	});
}

void panel_render_system::render_settler_mode() {
	term(1)->box(1, 2, 73, 60, WHITE, BLACK, true);
	for (int i=3; i<60; ++i) term(1)->print(2, i, "                                                                        ");

	name_t * name = entity(selected_settler)->component<name_t>();
	game_stats_t * stats = entity(selected_settler)->component<game_stats_t>();
	species_t * species = entity(selected_settler)->component<species_t>();
	settler_ai_t * ai = entity(selected_settler)->component<settler_ai_t>();
	health_t * health = entity(selected_settler)->component<health_t>();

	std::stringstream header;
	header << name->first_name << " " << name->last_name << " (" << stats->profession_tag << ")";
	std::stringstream header2;
	header2 << species->gender_str() << ", " << species->sexuality_str() << ", " << stats->age << " years old. "
		<< species->height_feet() << ", " << species->weight_lbs();

	term(1)->print(2, 4, header.str(), YELLOW, BLACK );
	term(1)->print(2, 5, header2.str(), WHITE, BLACK );
	term(1)->print(2, 6, ai->job_status);

	term(1)->print(2, 8, "Hit Points: " + std::to_string(health->current_hitpoints) + std::string("/") + std::to_string(health->max_hitpoints));
	int y = 9;
	for (const health_part_t &part : health->parts) {
		std::string part_state = "OK";
		if (part.current_hitpoints < 1 && part.current_hitpoints > -10) part_state = "IMPAIRED";
		if (part.current_hitpoints < -9 && part.current_hitpoints > -20) part_state = "BROKEN";
		if (part.current_hitpoints < -19) part_state = "GONE";
		term(1)->print(2,y, part.part + std::string(": ") + std::to_string(part.current_hitpoints) + std::string("/") + std::to_string(part.max_hitpoints) + std::string(" (") + part_state + std::string(")"));
		++y;
	}

	term(1)->print(30, 8, species->gender_pronoun() + std::string(" ") + stats->strength_str(), WHITE, BLACK);
	term(1)->print(30, 9, species->gender_pronoun() + std::string(" ") + stats->dexterity_str(), WHITE, BLACK);
	term(1)->print(30, 10, species->gender_pronoun() + std::string(" ") + stats->constitution_str(), WHITE, BLACK);
	term(1)->print(30, 11, species->gender_pronoun() + std::string(" ") + stats->intelligence_str(), WHITE, BLACK);
	term(1)->print(30, 12, species->gender_pronoun() + std::string(" ") + stats->wisdom_str(), WHITE, BLACK);
	term(1)->print(30, 13, species->gender_pronoun() + std::string(" ") + stats->charisma_str(), WHITE, BLACK);
	term(1)->print(30, 14, species->gender_pronoun() + std::string(" ") + stats->comeliness_str(), WHITE, BLACK);
	term(1)->print(30, 15, species->gender_pronoun() + std::string(" ") + stats->ethics_str(), WHITE, BLACK);

	y=17;
	for (const auto &skill : stats->skills) {
		term(1)->print(30, y, skill.first + std::string(" : ") + std::to_string(skill.second.skill_level));
	}
	++y;

	each<item_t, item_carried_t>([&y, this] (entity_t &e, item_t &item, item_carried_t &carried) {
		if (carried.carried_by == selected_settler) {
			term(1)->print(30, y, item.item_name);
			++y;
		}
	});

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