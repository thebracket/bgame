#include "mode_play_system.hpp"
#include "../../main/game_globals.hpp"
#include "gui_system.hpp"
#include "../input/mouse_input_system.hpp"
#include "gui_system.hpp"
#include "../../messages/build_request_message.hpp"
#include <rltk.hpp>
#include "../../components/building.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/item.hpp"
#include "../../components/construct_container.hpp"
#include "../../components/construct_provides_door.hpp"
#include "../../components/item_stored.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/plants.hpp"

using namespace rltk;
using namespace rltk::colors;

void mode_play_system::configure() {
    system_name = "Play mode";
}

void mode_play_system::update(const double duration_ms) {
    term(4)->clear();
	if (game_master_mode == TILEMENU) show_tilemenu();
    if (game_master_mode != PLAY && game_master_mode != ROGUE) return;

    // Controls Help
	if (pause_mode == PAUSED) {
        add_gui_element(std::make_unique<map_static_text>( 54, 1, "SPACE", YELLOW));
        add_gui_element(std::make_unique<map_static_text>( 60, 1, "Unpause"));
	} else {
        add_gui_element(std::make_unique<map_static_text>( 54, 1, "SPACE", YELLOW));
        add_gui_element(std::make_unique<map_static_text>( 60, 1, "Pause"));
	}
    add_gui_element(std::make_unique<map_static_text>( 68, 1, "/", YELLOW));
    add_gui_element(std::make_unique<map_static_text>( 71, 1, "One Step"));

	// Since we're using an 8x8, it's just a matter of dividing by 8 to find the terminal-character
	// coordinates. There will be a helper function for this once we get into retained GUIs.
	const int world_x = std::min(clip_left + mouse::term1x, REGION_WIDTH);
	const int world_y = std::min(clip_top + mouse::term1y-2, REGION_HEIGHT);
    if (world_x < 0 || world_x > REGION_WIDTH-1 || world_y < 0 || world_y > REGION_HEIGHT-1) return;
	const int tile_idx = mapidx( world_x, world_y, camera_position->region_z );
	bool tooltip = false;
	
	if (mouse::term1x == last_mouse_x && mouse::term1y == last_mouse_y && world_y > 0) {
		mouse_dwell_time += (float)duration_ms;
		if (mouse_dwell_time > 200.0 && tile_idx !=0 && current_region->revealed[tile_idx] ) tooltip = true;
	} else {
		last_mouse_x = mouse::term1x;
		last_mouse_y = mouse::term1y;
		mouse_dwell_time = 0.0;
	}

	if (mouse::clicked && game_master_mode == PLAY) {
		pause_mode = PAUSED;
		game_master_mode = TILEMENU;
		selected_tile_x = world_x;
		selected_tile_y = world_y;
		selected_tile_z = camera_position->region_z;
		menu_x = mouse::term4x;
		menu_y = mouse::term4y;
	} else if (tooltip) {
		show_tooltip(world_x, world_y, tile_idx);
	}
}

void mode_play_system::show_tooltip(const int world_x, const int world_y, const int tile_idx) {
	std::vector<std::string> lines;

	{
		std::stringstream ss;
		if (current_region->solid[tile_idx]) ss << "Solid-";
		if (current_region->opaque[tile_idx]) ss << "Opaque-";
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
			case tile_type::SOLID : ss << "Solid Rock (" << material_name(current_region->tile_material[tile_idx]) << ")"; break;
			case tile_type::OPEN_SPACE : ss << "Open Space"; break;
			case tile_type::WALL : ss << "Wall (" << material_name(current_region->tile_material[tile_idx]) << ")"; break;
			case tile_type::RAMP : ss << "Ramp (" << material_name(current_region->tile_material[tile_idx]) << ")"; break;
			case tile_type::STAIRS_UP : ss << "Up Stairs (" << material_name(current_region->tile_material[tile_idx]) << ")"; break;
			case tile_type::STAIRS_DOWN : ss << "Down Stairs (" << material_name(current_region->tile_material[tile_idx]) << ")"; break;
			case tile_type::STAIRS_UPDOWN : ss << "Spiral Stairs (" << material_name(current_region->tile_material[tile_idx]) << ")"; break;
			case tile_type::FLOOR : ss << "Floor (" << material_name(current_region->tile_material[tile_idx]) << ")"; break;
			case tile_type::TREE_TRUNK : ss << "Tree Trunk"; break;
			case tile_type::TREE_LEAF : ss << "Tree Foliage"; break;
			case tile_type::WINDOW : ss << "Window"; break;
			case tile_type::CLOSED_DOOR : ss << "Closed Door (" << material_name(current_region->tile_material[tile_idx]) << ")"; break;
			default : ss << "Unknown!";
		}
		lines.push_back(ss.str());
	}
	if (current_region->tile_type[tile_idx] == tile_type::FLOOR && !current_region->tile_flags[tile_idx].test(CONSTRUCTION)) {
		if (current_region->tile_vegetation_type[tile_idx] > 0) {
			std::stringstream ss;
            plant_t plant = get_plant_def(current_region->tile_vegetation_type[tile_idx]);
            ss << plant.name << " (";
            switch (current_region->tile_vegetation_lifecycle[tile_idx]) {
                case 0 : ss << "Germinating"; break;
                case 1 : ss << "Sprouting"; break;
                case 2 : ss << "Growing"; break;
                case 3 : ss << "Flowering"; break;
                default : ss << "Unknown - error!";
            }
            const std::string harvest_to = plant.provides[current_region->tile_vegetation_lifecycle[tile_idx]];
            if (harvest_to != "none") ss << " - " << harvest_to;
            ss << ")";
			lines.push_back(ss.str());
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
		bool on_building = false;

		if (pos.z == camera_position->region_z) {
			if (pos.x == world_x && pos.y == world_y) {
				on_building = true;
			} else {
				// Special case because 3x3 use the mid-point as center
				if (building.height == 3 && building.width == 3 && world_x >= pos.x-1 && world_x <= pos.x+1 && world_y >= pos.y-1 && world_y <= pos.y+1) {
					on_building = true;
				} else if (building.height != 1 && building.width != 1 && world_x >= pos.x && world_x <= pos.x + building.width && world_y >= pos.y && world_y <= pos.y + building.height) {
					on_building = true;
				}
			}
		}

		if (on_building) {
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
	if (current_region->stockpile_id[mapidx(world_x, world_y, camera_position->region_z)]>0) {
        lines.push_back(std::string("Stockpile #")+std::to_string(current_region->stockpile_id[mapidx(world_x, world_y, camera_position->region_z)]));
    }

	int longest = 0;
	for (const std::string &s : lines) {
		if (s.size() > longest) longest = (int)s.size();
	}

	// TODO - dynamic placement
	float revealed_pct = mouse_dwell_time - 200.0F;
	revealed_pct /= game_config.tooltip_speed;
	if (revealed_pct > 1.0) revealed_pct = 1.0F;
	if (!game_config.tooltip_fadein) revealed_pct = 1.0F;

	if (revealed_pct < 1.0) {
		for (std::string &s : lines) {
			int n_garbled = static_cast<int>(s.size() - ((float)s.size() * (revealed_pct/2.0F)));
			for (int i=0; i<n_garbled; ++i) s[i] = static_cast<uint8_t>(rng.roll_dice(1,255));
		}
	}

	bool right_align = true;
	if (mouse::term1x > term(1)->term_width/2 ) right_align = false;
	int tt_y = mouse::term4y;
	if (tt_y+lines.size() > term(4)->term_height-1) tt_y -= (int)lines.size()+1;

	if (right_align) {
		auto color = lerp(BLACK, LIGHT_GREEN, revealed_pct);
		int tt_x = mouse::term4x+2;
		term(4)->set_char(mouse::term4x+1, mouse::term4y, vchar{27, color, DARKEST_GREEN});
		term(4)->box(tt_x, tt_y, longest+1, (int)lines.size()+1, color);
		++tt_y;
		for (const std::string &s : lines) {
			term(4)->print(tt_x+1, tt_y, s, color);
			++tt_y;
		}
	} else {
		auto color = lerp(BLACK, LIGHT_GREEN, revealed_pct);
		int tt_x = mouse::term4x-3-longest;
		term(4)->box(tt_x, tt_y, longest+1, (int)lines.size()+1, color);
		++tt_y;
		for (const std::string &s : lines) {
			term(4)->print(tt_x+1, tt_y, s, color);
			++tt_y;
		}
		term(4)->set_char(mouse::term4x-1, mouse::term4y, vchar{26, color, DARKEST_GREEN});
	}
}

void mode_play_system::show_tilemenu() {
	std::unique_ptr<gui_popup_menu> menu = std::make_unique<gui_popup_menu>(menu_x, menu_y, true, true, [] () { game_master_mode=PLAY; });

	each<name_t, position_t, settler_ai_t>([&menu] (entity_t &entity, name_t &name, position_t &pos, settler_ai_t &settler) {
		if (pos.x == selected_tile_x && pos.y == selected_tile_y && pos.z == selected_tile_z) {
			boost::optional<std::function<void()>> on_click{};
			on_click = [&entity] () {
				selected_settler = entity.id;
				game_master_mode = SETTLER;
			};
			menu->options.push_back(std::make_pair(name.first_name + std::string(" ") + name.last_name, on_click));
		}
	});

	each<name_t, position_t, sentient_ai>([&menu] (entity_t &entity, name_t &name, position_t &pos, sentient_ai &settler) {
		if (pos.x == selected_tile_x && pos.y == selected_tile_y && pos.z == selected_tile_z) {
			boost::optional<std::function<void()>> on_click{};
			on_click = [&entity] () {
				selected_settler = entity.id;
				game_master_mode = SENTIENT_INFO;
			};
			menu->options.push_back(std::make_pair(name.first_name + std::string(" ") + name.last_name, on_click));
		}
	});

	each<name_t, position_t, grazer_ai>([&menu] (entity_t &entity, name_t &name, position_t &pos, grazer_ai &settler) {
		if (pos.x == selected_tile_x && pos.y == selected_tile_y && pos.z == selected_tile_z) {
			boost::optional<std::function<void()>> on_click{};
			on_click = [&entity] () {
				selected_settler = entity.id;
				game_master_mode = GRAZER_INFO;
			};
			menu->options.push_back(std::make_pair(name.first_name + std::string(" ") + name.last_name, on_click));
		}
	});

	each<building_t, position_t>([&menu] (entity_t &building_entity, building_t &building, position_t &pos) {
		bool on_building = false;

		if (pos.z == selected_tile_z) {
			if (pos.x == selected_tile_x && pos.y == selected_tile_y) {
				on_building = true;
			} else {
				// Special case because 3x3 use the mid-point as center
				if (building.height == 3 && building.width == 3 && selected_tile_x >= pos.x-1 && selected_tile_x <= pos.x+1 && selected_tile_y >= pos.y-1 && selected_tile_y <= pos.y+1) {
					on_building = true;
				} else if (building.height != 1 && building.width !=1 && selected_tile_x >= pos.x && selected_tile_x <= pos.x + building.width && selected_tile_y >= pos.y && selected_tile_y <= pos.y + building.height) {
					on_building = true;
				}
			}
		}

		if (on_building) {
			// It's building and we can see it			
			auto finder = building_defs.find(building.tag);
			std::string building_name = "Unknown Building";
			if (finder != building_defs.end()) {

				if (building.complete) {
					bool is_being_removed = false;
					for (const auto &d : designations->deconstructions) {
						if (d.building_id == building_entity.id) is_being_removed = true;
					}
					each<settler_ai_t>([&is_being_removed, &building_entity] (entity_t &E, settler_ai_t &ai) {
						if (ai.job_type_major == JOB_DECONSTRUCT && ai.target_id == building_entity.id) is_being_removed = true;
					});
					building_name = finder->second.name;

					if (!is_being_removed) {
						boost::optional<std::function<void()>> on_click{};
						on_click = [&building_entity] () {
							designations->deconstructions.push_back(unbuild_t{true, building_entity.id});
							game_master_mode = PLAY;
						};
						if (building_name != "Cordex") { // Don't let the player commit suicide this easily!
							menu->options.push_back(std::make_pair(std::string("Deconstruct ")+building_name, on_click));
						}
					} else {
						boost::optional<std::function<void()>> on_click{};
						on_click = [&building_entity] () {
							designations->deconstructions.erase(
								// Remove from designations
								std::remove_if(designations->deconstructions.begin(),
									designations->deconstructions.end(),
									[&building_entity] (unbuild_t u) { return (u.is_building == true && u.building_id == building_entity.id); }),
								designations->deconstructions.end()
							);

							// See if there is a settler trying to do this task
							each<settler_ai_t>([&building_entity] (entity_t &E, settler_ai_t &ai) {
								if (ai.job_type_major == JOB_DECONSTRUCT && ai.target_id == building_entity.id) {
									ai.job_type_major = JOB_IDLE;
								}
							});
							game_master_mode = PLAY;
						};
						menu->options.push_back(std::make_pair(std::string("Cancel removal of ")+building_name, on_click));
					}

					// Doors
					auto door = building_entity.component<construct_door_t>();
					if (door && building.civ_owner == 0) {
						boost::optional<std::function<void()>> on_lock{};
						on_lock = [&building_entity] () {
							building_entity.component<construct_door_t>()->locked = true;
							emit_deferred(door_changed_message{});
						};
						boost::optional<std::function<void()>> on_unlock{};
						on_unlock = [&building_entity] () {
							building_entity.component<construct_door_t>()->locked = false;
							emit_deferred(door_changed_message{});
						};
						if (!door->locked) {
							menu->options.push_back(std::make_pair(std::string("Lock ")+building_name, on_lock));
						} else {
							menu->options.push_back(std::make_pair(std::string("Unlock ")+building_name, on_unlock));
						}
					}
				} else {
					building_name = finder->second.name;
					boost::optional<std::function<void()>> on_click{};
					on_click = [&building_entity] () {
						emit(cancel_build_request_message{building_entity.id});
					};
					menu->options.push_back(std::make_pair(std::string("Cancel construction of ")+building_name, on_click));
				}
			}
		}
	});

	const auto idx = mapidx(selected_tile_x, selected_tile_y, selected_tile_z);
	if ((current_region->tile_type[idx] == tile_type::WALL || current_region->tile_type[idx] == tile_type::FLOOR 
		|| current_region->tile_type[idx] == tile_type::STAIRS_DOWN || current_region->tile_type[idx] == tile_type::STAIRS_UP 
		|| current_region->tile_type[idx] == tile_type::STAIRS_UPDOWN) && current_region->tile_flags[idx].test(CONSTRUCTION) )
	{
		boost::optional<std::function<void()>> on_click{};
		on_click = [idx] () {
			designations->deconstructions.push_back(unbuild_t{false,static_cast<std::size_t>(idx)});
			game_master_mode = PLAY;
		};
		menu->options.push_back(std::make_pair("Demolish Structure", on_click));
	}

	if (menu->options.empty()) {
		game_master_mode = PLAY;
	} else {
		add_gui_element(std::move(menu));
	}
}
