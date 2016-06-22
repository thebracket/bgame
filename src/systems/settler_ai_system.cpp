#include "settler_ai_system.hpp"
#include "../messages/messages.hpp"
#include "../game_globals.hpp"
#include "path_finding.hpp"
#include "mining_system.hpp"
#include "inventory_system.hpp"
#include "workflow_system.hpp"
#include <iostream>
#include <map>

using namespace rltk;

void settler_ai_system::update(const double duration_ms) {
}

void settler_ai_system::settler_calculate_initiative(settler_ai_t &ai, game_stats_t &stats) {
	ai.initiative = std::max(1, rng.roll_dice(1, 6) - stat_modifier(stats.dexterity));
}

void settler_ai_system::wander_randomly(entity_t &entity, position_t &original) {
	position_t pos = original;
	const int tile_index = mapidx(pos.x, pos.y, pos.z);
	const int direction = rng.roll_dice(1,6);
	switch (direction) {
		case 1 : if (current_region->tiles[tile_index].flags.test(tile_flags::CAN_GO_UP)) pos.z++; break;
		case 2 : if (current_region->tiles[tile_index].flags.test(tile_flags::CAN_GO_DOWN)) pos.z--; break;
		case 3 : if (current_region->tiles[tile_index].flags.test(tile_flags::CAN_GO_NORTH)) pos.y--; break;
		case 4 : if (current_region->tiles[tile_index].flags.test(tile_flags::CAN_GO_SOUTH)) pos.y++; break;
		case 5 : if (current_region->tiles[tile_index].flags.test(tile_flags::CAN_GO_EAST)) pos.x++; break;
		case 6 : if (current_region->tiles[tile_index].flags.test(tile_flags::CAN_GO_WEST)) pos.x--; break;
	}
	if (current_region->tiles[tile_index].flags.test(tile_flags::SOLID)) { 
		pos = original;
	} else {
		move_to(entity, original, pos);
	}

	renderable_t * render = entity.component<renderable_t>();
	render->foreground = rltk::colors::YELLOW;
	render->glyph = '@';
}

void settler_ai_system::configure() {
	subscribe<tick_message>([this](tick_message &msg) {
		each<settler_ai_t, game_stats_t, species_t, position_t, name_t>([this] (entity_t &entity, settler_ai_t &ai, game_stats_t &stats, 
			species_t &species, position_t &pos, name_t &name) 
		{
			if (ai.initiative < 1) {
				const int shift_id = ai.shift_id;
				const int hour_of_day = calendar->hour;
				const shift_type_t current_schedule = calendar->defined_shifts[shift_id].hours[hour_of_day];

				switch (current_schedule) {
					case SLEEP_SHIFT : this->do_sleep_time(entity, ai, stats, species, pos, name); break;
					case LEISURE_SHIFT : this->do_leisure_time(entity, ai, stats, species, pos, name); break;
					case WORK_SHIFT : this->do_work_time(entity, ai, stats, species, pos, name); break;
				}
				
				this->settler_calculate_initiative(ai, stats);
			} else {
				--ai.initiative;
			}
		});
	});
}

void settler_ai_system::move_to(entity_t &e, position_t &pos, position_t &destination) {
	pos.x = destination.x;
	pos.y = destination.y;
	pos.z = destination.z;
	emit(entity_moved_message{e.id, destination});
	emit(renderables_changed_message{});
}

void settler_ai_system::drop_current_tool(entity_t &e, settler_ai_t &ai, position_t &pos) {
	emit(item_claimed_message{ai.current_tool, false});
	try { delete_component<item_carried_t>(ai.current_tool); } catch (...) {}
	try { entity(ai.current_tool)->assign(position_t{ pos.x, pos.y, pos.z }); } catch (...) {}
	emit(inventory_changed_message{});	
	ai.current_tool = 0;
}

void settler_ai_system::change_settler_glyph(entity_t &e, const vchar &render_as) {
	renderable_t * render = e.component<renderable_t>();
	render->foreground = render_as.foreground;
	render->background = render_as.background;
	render->glyph = render_as.glyph;
	emit(renderables_changed_message{});
}

void settler_ai_system::become_idle(entity_t &e, settler_ai_t &ai, name_t &name) {
	ai.job_type_major = JOB_IDLE;
	ai.target_x = 0; 
	ai.target_y = 0; 
	ai.target_z = 0;
	change_settler_glyph(e, vchar{'@', rltk::colors::YELLOW, rltk::colors::BLACK});
	change_job_status(ai, name, "Idle");
}

void settler_ai_system::cancel_action(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name, const std::string reason) {
	// Drop whatever we are doing!
	if (ai.job_type_major == JOB_SLEEP) {
		each<construct_provides_sleep_t, position_t>([&pos] (entity_t &e, construct_provides_sleep_t &bed, position_t &bed_pos) {
			if (bed_pos == pos) bed.claimed = false;
		});
	}
	// Drop tool
	if ((ai.job_type_major == JOB_MINE && ai.current_tool != 0) || (ai.job_type_major == JOB_CHOP && ai.current_tool != 0)) {
		drop_current_tool(e, ai, pos);
	}
	// Drop components if necessary
	if (ai.job_type_major == JOB_CONSTRUCTION) {
		// Put the job back on the available jobs list
		if (ai.building_target) {
			designations->buildings.push_back(ai.building_target.get());
			ai.building_target.reset();
		}

		// If holding a component, drop it
		if (ai.current_tool != 0) drop_current_tool(e, ai, pos);
	}

	std::cout << name.first_name << " cancels action: " << reason << "\n";
	become_idle(e, ai, name);
}

void settler_ai_system::do_sleep_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_major != JOB_IDLE && ai.job_type_major != JOB_SLEEP) {
		cancel_action(entity, ai, stats, species, pos, name, "Bed time!");
		return;
	}
	if (ai.job_type_major == JOB_IDLE) {
		ai.job_type_major = JOB_SLEEP;
		ai.job_type_minor = JM_FIND_BED;
		change_job_status(ai, name, "Looking for a bed");
		return;
	}
	if (ai.job_type_major != JOB_SLEEP) throw std::runtime_error("Sleep mode, but shouldn't have made it to here.\n");

	if (ai.job_type_minor == JM_FIND_BED) {
		// Find nearest unclaimed bed
		std::map<float, std::pair<construct_provides_sleep_t,position_t>> bed_candidates;
		each<construct_provides_sleep_t, position_t>([&bed_candidates, &pos] (entity_t &e, construct_provides_sleep_t &bed, position_t &bed_pos) {
			if (!bed.claimed) {
				bed_candidates[distance3d(pos.x, pos.y, pos.z, bed_pos.x, bed_pos.y, bed_pos.z)] = std::make_pair(bed, bed_pos);
			}
		});
		if (bed_candidates.empty()) std::cout << "Warning: no bed found. We should implement ground sleeping.\n";

		// Claim it
		bed_candidates.begin()->second.first.claimed = true;
		ai.target_x = bed_candidates.begin()->second.second.x;
		ai.target_y = bed_candidates.begin()->second.second.y;
		ai.target_z = bed_candidates.begin()->second.second.z;

		// Set the path
		ai.current_path = find_path(pos, position_t{ai.target_x, ai.target_y, ai.target_z});
		if (!ai.current_path->success) {
			std::cout << "Warning: no path to bed found.\n";
			ai.current_path.reset();
		}
		ai.job_type_minor = JM_GO_TO_BED;
		change_job_status(ai, name, "Going to bed");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_BED) {
		if (!ai.current_path) {
			ai.job_type_minor = JM_FIND_BED;
			change_job_status(ai, name, "Looking for a bed");
			std::cout << "No path to bed - trying again to find one.\n";
			return;
		}
		if (pos == ai.current_path->destination) {
			ai.job_type_minor = JM_SLEEP;
			change_job_status(ai, name, "Sleeping");
			return;
		}
		position_t next_step = ai.current_path->steps.front();
		move_to(entity, pos, next_step);
		ai.current_path->steps.pop_front();
		emit(renderables_changed_message{});
		return;
	}

	if (ai.job_type_minor == JM_SLEEP) {
		renderable_t * render = entity.component<renderable_t>();
		render->foreground = rltk::colors::BLUE;

		if (rng.roll_dice(1,6) < 3) {
			render->glyph = '@';
		} else {
			render->glyph = 'Z';
		}

		emit(renderables_changed_message{});
	}
}

void settler_ai_system::do_leisure_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_major == JOB_SLEEP) {
		cancel_action(entity, ai, stats, species, pos, name, "Time to wake up");
		return;
	}
	wander_randomly(entity, pos);
}

inline rltk::color_t get_task_color(const std::string &skill) {
	if (skill == "Masonry") {
		return rltk::colors::LightGrey;
	} else if (skill == "Carpentry") {
		return rltk::colors::BurlyWood;
	}
	return rltk::colors::Grey;
}

void settler_ai_system::do_work_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_major == JOB_SLEEP) {
		cancel_action(entity, ai, stats, species, pos, name, "Time to wake up");
		return;
	}
	if (ai.job_type_major == JOB_IDLE) {
		// Find something to do!
		const int idx = mapidx(pos.x, pos.y, pos.z);
		
		if (ai.permitted_work[JOB_MINING] && mining_map[idx]<250 && is_item_category_available(TOOL_DIGGING)) {
			change_settler_glyph(entity, vchar{'@', rltk::colors::WHITE, rltk::colors::BLACK});
			ai.job_type_major = JOB_MINE;
			ai.job_type_minor = JM_FIND_PICK;
			change_job_status(ai, name, "Finding mining tools.");
			return;
		}
		if (ai.permitted_work[JOB_CHOPPING] && designations->chopping.size() > 0 && is_item_category_available(TOOL_CHOPPING)) {
			change_settler_glyph(entity, vchar{'@', rltk::colors::Brown, rltk::colors::BLACK});
			ai.job_type_major = JOB_CHOP;
			ai.job_type_minor = JM_FIND_AXE;
			change_job_status(ai, name, "Finding chopping tools.");
			return;
		}
		if (ai.permitted_work[JOB_CONSTRUCTION] && designations->buildings.size() > 0) {
			ai.building_target.reset();

			ai.building_target = (designations->buildings.back());
			designations->buildings.pop_back();

			if (ai.building_target) {
				change_settler_glyph(entity, vchar{'@', rltk::colors::Pink, rltk::colors::BLACK});
				ai.job_type_major = JOB_CONST;
				ai.job_type_minor = JM_SELECT_COMPONENT;
				change_job_status(ai, name, "Reading building plans.");
			}
			return;
		}

		// Look for an automatic reaction to perform
		boost::optional<reaction_task_t> autojob = find_automatic_reaction_task(ai);
		if (autojob) {
			auto finder = reaction_defs.find(autojob.get().reaction_tag);
			change_settler_glyph(entity, vchar{'@', get_task_color(finder->second.skill), rltk::colors::BLACK});
			ai.job_type_major = JOB_REACTION;
			ai.job_type_minor = JM_SELECT_INPUT;
			change_job_status(ai, name, autojob.get().job_name);
			ai.reaction_target = autojob;
			return;
		}
	} else if (ai.job_type_major == JOB_MINE) {
		do_mining(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_CHOP) {
		do_chopping(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_CONST) {
		do_building(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_REACTION) {
		do_reaction(entity, ai, stats, species, pos, name);
		return;
	}
	//wander_randomly(entity, pos);
}

void settler_ai_system::change_job_status(settler_ai_t &ai, name_t &name, const std::string new_status) {
	ai.job_status = new_status;
	std::cout << name.first_name << " is now: " << new_status << "\n";
}

void settler_ai_system::do_mining(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	//std::cout << name.first_name << ": " << ai.job_status << "\n";

	if (ai.job_type_minor == JM_FIND_PICK) {
		inventory_item_t pick = claim_closest_item_by_category(TOOL_DIGGING, pos);
		if (!pick.pos) {
			cancel_action(e, ai, stats, species, pos, name, "No available pick");
			return;
		}
		ai.current_path = find_path(pos, pick.pos.get());
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No available pick");
			return;
		}
		ai.job_type_minor = JM_GO_TO_PICK;
		ai.target_x = pick.pos.get().x;
		ai.target_y = pick.pos.get().y;
		ai.target_z = pick.pos.get().z;
		change_job_status(ai, name, "Traveling to digging tool");
		ai.current_tool = pick.id;
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_PICK) {
		if (pos == ai.current_path->destination) {
			// We're at the pick
			ai.current_path.reset();
			ai.job_type_minor = JM_COLLECT_PICK;
			change_job_status(ai, name, "Collect digging tool");
			return;
		}
		// Travel to pick
		position_t next_step = ai.current_path->steps.front();
		move_to(e, pos, next_step);
		ai.current_path->steps.pop_front();
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_PICK) {
		// Find the pick, remove any position or stored components, add a carried_by component
		try { delete_component<position_t>(ai.current_tool); } catch (...) {}
		try { delete_component<item_stored_t>(ai.current_tool); } catch (...) {}
		entity(ai.current_tool)->assign(item_carried_t{ INVENTORY, e.id });

		// Notify the inventory system of the change	
		emit(inventory_changed_message{});
		emit(renderables_changed_message{});

		ai.job_type_minor = JM_GO_TO_SITE;
		change_job_status(ai, name, "Travelling to dig site");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_SITE) {
		const int idx = mapidx(pos.x, pos.y, pos.z);
		if (mining_map[idx]==0) {
			// We're at a diggable site
			ai.job_type_minor = JM_DIG;
			change_job_status(ai, name, "Digging");
			return;
		}
		// Look at adjacent mining map entries, and path to the closest one. If there is no option,
		// Drop the pick.
		int current_direction = 0;
		int min_value = 512;
		if (mining_map[mapidx(pos.x, pos.y-1, pos.z)] < min_value && current_region->tiles[idx].flags.test(tile_flags::CAN_GO_NORTH)) { 
			min_value = mining_map[mapidx(pos.x, pos.y-1, pos.z)]; 
			current_direction = 1; 
		}
		if (mining_map[mapidx(pos.x, pos.y+1, pos.z)] < min_value && current_region->tiles[idx].flags.test(tile_flags::CAN_GO_SOUTH)) { 
			min_value = mining_map[mapidx(pos.x, pos.y+1, pos.z)]; 
			current_direction = 2; 
		}
		if (mining_map[mapidx(pos.x-1, pos.y, pos.z)] < min_value && current_region->tiles[idx].flags.test(tile_flags::CAN_GO_WEST)) { 
			min_value = mining_map[mapidx(pos.x-1, pos.y, pos.z)]; 
			current_direction = 3; 
		}
		if (mining_map[mapidx(pos.x+1, pos.y, pos.z)] < min_value && current_region->tiles[idx].flags.test(tile_flags::CAN_GO_EAST)) { 
			min_value = mining_map[mapidx(pos.x+1, pos.y, pos.z)]; 
			current_direction = 4; 
		}
		if (mining_map[mapidx(pos.x, pos.y, pos.z-1)] < min_value && current_region->tiles[idx].flags.test(tile_flags::CAN_GO_DOWN)) { 
			min_value = mining_map[mapidx(pos.x, pos.y, pos.z-1)]; 
			current_direction = 5; 
		}
		if (mining_map[mapidx(pos.x, pos.y, pos.z+1)] < min_value && current_region->tiles[idx].flags.test(tile_flags::CAN_GO_UP)) { 
			min_value = mining_map[mapidx(pos.x, pos.y, pos.z+1)]; 
			current_direction = 6; 
		}

		if (current_direction == 0) {
			ai.job_type_minor = JM_DROP_PICK;
			change_job_status(ai, name, "Dropping digging tools due to being lost!");
			return;
		}

		position_t original = pos;
		switch (current_direction) {
			case 1 : --pos.y; break;
			case 2 : ++pos.y; break;
			case 3 : --pos.x; break;
			case 4 : ++pos.x; break;
			case 5 : --pos.z; break;
			case 6 : ++pos.z; break;
		}
		move_to(e, original, pos);

		return;
	}

	if (ai.job_type_minor == JM_DIG) {
		auto skill_check = skill_roll(stats, rng, "Mining", DIFICULTY_TOUGH);
		if (skill_check >= SUCCESS) {
			// Determine the digging target from here
			// Make a skill roll, and if successful complete the action
			// When complete, move to dropping the pick
			const int idx = mapidx(pos.x, pos.y, pos.z);
			const int target_idx = mining_targets[idx];
			const int target_operation = designations->mining[target_idx];

			if (target_operation == 1) {
				// Dig
				current_region->tiles[target_idx].flags.reset(tile_flags::SOLID);
				current_region->tiles[target_idx].flags.set(tile_flags::CONSTRUCTION);
				current_region->tiles[target_idx].contents = 4101;
				current_region->calculate_render_tile(target_idx);
				if (current_region->tiles[target_idx].base_type==2 && rng.roll_dice(1,4)>=2) {
					spawn_item_on_ground(pos.x, pos.y, pos.z, "stone_boulder");
				}
			} else if (target_operation == 2) {
				// Channel
				current_region->tiles[target_idx].flags.reset(tile_flags::SOLID);
				current_region->tiles[target_idx].contents = 0;
				current_region->tiles[target_idx].base_type = 0;
				current_region->calculate_render_tile(target_idx);
				
				// Add ramp
				const int below = target_idx - (REGION_WIDTH * REGION_HEIGHT);
				current_region->tiles[below].flags.reset(tile_flags::SOLID);
				current_region->tiles[below].flags.set(tile_flags::CONSTRUCTION);
				current_region->tiles[below].contents = 4118;
				current_region->calculate_render_tile(below);
			} else if (target_operation == 3) {
				// Ramp
				current_region->tiles[target_idx].flags.reset(tile_flags::SOLID);
				current_region->tiles[target_idx].flags.set(tile_flags::CONSTRUCTION);
				current_region->tiles[target_idx].contents = 4118;
				current_region->calculate_render_tile(target_idx);

				const int above = target_idx + (REGION_WIDTH * REGION_HEIGHT);
				current_region->tiles[above].flags.reset(tile_flags::SOLID);
				current_region->tiles[above].base_type = 0;
				current_region->tiles[above].contents = 0;
				current_region->calculate_render_tile(above);
				
			} else if (target_operation == 4) {
				// Up
				current_region->tiles[target_idx].flags.reset(tile_flags::SOLID);
				current_region->tiles[target_idx].flags.set(tile_flags::CONSTRUCTION);
				current_region->tiles[target_idx].contents = 4111;
				current_region->calculate_render_tile(target_idx);
			} else if (target_operation == 5) {
				// Down
				current_region->tiles[target_idx].flags.reset(tile_flags::SOLID);
				current_region->tiles[target_idx].flags.set(tile_flags::CONSTRUCTION);
				current_region->tiles[target_idx].contents = 4110;
				current_region->calculate_render_tile(target_idx);
			} else if (target_operation == 6) {
				// UpDown
				current_region->tiles[target_idx].flags.reset(tile_flags::SOLID);
				current_region->tiles[target_idx].flags.set(tile_flags::CONSTRUCTION);
				current_region->tiles[target_idx].contents = 4109;
				current_region->calculate_render_tile(target_idx);
			}

			for (int Z=-2; Z<3; ++Z) {
				for (int Y=-2; Y<3; ++Y) {
					for (int X=-2; X<3; ++X) {
						current_region->determine_tile_standability(pos.x + X, pos.y + Y, pos.z + Z);
						current_region->determine_tile_connectivity(pos.x + X, pos.y + Y, pos.z + Z);
					}
				}
			}

			designations->mining.erase(target_idx);
			emit(recalculate_mining_message{});
			emit(map_dirty_message{});
			ai.job_type_minor = JM_DROP_PICK;
			change_job_status(ai, name, "Dropping digging tools - work complete");
			return;
		} else {
			// Failed!
			// if (skill_check == CRITICAL_FAIL) inflictDamage!
			return;
		}
	}

	if (ai.job_type_minor == JM_DROP_PICK) {
		if (ai.current_tool == 0) std::cout << "Warning: pick is unassigned at this time\n";
		drop_current_tool(e, ai, pos);

		become_idle(e, ai, name);
		return;
	}
}

void settler_ai_system::do_chopping(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	//std::cout << name.first_name << ": " << ai.job_status << "\n";

	if (ai.job_type_minor == JM_FIND_AXE) {
		inventory_item_t axe = claim_closest_item_by_category(TOOL_CHOPPING, pos);
		if (!axe.pos) {
			cancel_action(e, ai, stats, species, pos, name, "No available axe");
			return;
		}
		ai.current_path = find_path(pos, axe.pos.get());
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No available axe");
			return;
		}
		ai.job_type_minor = JM_GO_TO_AXE;
		ai.target_x = axe.pos.get().x;
		ai.target_y = axe.pos.get().y;
		ai.target_z = axe.pos.get().z;
		change_job_status(ai, name, "Traveling to chopping tool");
		ai.current_tool = axe.id;
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_AXE) {
		if (pos == ai.current_path->destination) {
			// We're at the axe
			ai.current_path.reset();
			ai.job_type_minor = JM_COLLECT_AXE;
			change_job_status(ai, name, "Collect chopping tool");
			return;
		}
		// Travel to axe
		position_t next_step = ai.current_path->steps.front();
		move_to(e, pos, next_step);
		ai.current_path->steps.pop_front();
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_AXE) {
		// Find the pick, remove any position or stored components, add a carried_by component
		try { delete_component<position_t>(ai.current_tool); } catch (...) {}
		try { delete_component<item_stored_t>(ai.current_tool); } catch (...) {}
		entity(ai.current_tool)->assign(item_carried_t{ INVENTORY, e.id });

		// Notify the inventory system of the change	
		emit(inventory_changed_message{});
		emit(renderables_changed_message{});

		ai.job_type_minor = JM_FIND_TREE;
		change_job_status(ai, name, "Pathing to chopping site");
		return;
	}

	if (ai.job_type_minor == JM_FIND_TREE) {
		position_t tree_pos = designations->chopping.begin()->second;
		ai.target_id = designations->chopping.begin()->first;
		std::cout << "Targeting tree #" << ai.target_id << "\n";

		std::array<position_t, 4> target;
		target[0] = position_t{ tree_pos.x, tree_pos.y-1, tree_pos.z };
		target[1] = position_t{ tree_pos.x, tree_pos.y+1, tree_pos.z };
		target[2] = position_t{ tree_pos.x-1, tree_pos.y, tree_pos.z };
		target[3] = position_t{ tree_pos.x+1, tree_pos.y, tree_pos.z };

		int n = 0;
		while (!ai.current_path && n<4) {
			ai.current_path = find_path(pos, target[n]);
			if (ai.current_path->success) {
				ai.target_x = target[n].x;
				ai.target_y = target[n].y;
				ai.target_z = target[n].z;
			} else {
				ai.current_path.reset();
			}
			++n;
		}

		if (ai.current_path) {
			ai.job_type_minor = JM_GO_TO_TREE;
			change_job_status(ai, name, "Travel to chopping site");
			designations->chopping.erase(ai.target_id);
			return;
		} else {
			cancel_action(e, ai, stats, species, pos, name, "Can't find tree'");
			return;
		}
	}

	if (ai.job_type_minor == JM_GO_TO_TREE) {
		if (pos == ai.current_path->destination) {
			// We're at the tree
			ai.current_path.reset();
			ai.job_type_minor = JM_CHOP;
			change_job_status(ai, name, "Chopping tree");
			return;
		}
		// Travel to tree
		position_t next_step = ai.current_path->steps.front();
		move_to(e, pos, next_step);
		ai.current_path->steps.pop_front();
		return;
	}

	if (ai.job_type_minor == JM_CHOP) {
		// Chop down the tree
		auto skill_check = skill_roll(stats, rng, "Lumberjacking", DIFICULTY_TOUGH);

		if (skill_check >= SUCCESS) {
			// Tree is going down!
			int number_of_logs = 0;
			for (int z=0; z<REGION_DEPTH; ++z) {
				for (int y=0; y<REGION_HEIGHT; ++y) {
					for (int x=0; x<REGION_WIDTH; ++x) {
						const int idx = mapidx(x,y,z);
						if (current_region->tiles[idx].tree_id == ai.target_id) {
							current_region->tiles[idx].flags.reset(tile_flags::SOLID);
							current_region->tiles[idx].flags.reset(tile_flags::CAN_STAND_HERE);
							current_region->tiles[idx].flags.reset(tile_flags::TREE);
							current_region->tiles[idx].tree_id = 0;
							current_region->tiles[idx].base_type = 0;
							current_region->tiles[idx].contents = 0;
							current_region->calculate_render_tile(idx);
							++number_of_logs;
						}
					}
				}
			}
			const int tree_idx = mapidx(ai.target_x, ai.target_y, ai.target_z);
			current_region->tiles[tree_idx].base_type = 3;
			current_region->tiles[tree_idx].contents = 0;
			current_region->calculate_render_tile(tree_idx);			

			// Spawn wooden logs
			number_of_logs = (number_of_logs/20)+1;
			//std::cout << "We should spawn " << number_of_logs << " logs.\n";
			for (int i=0; i<number_of_logs; ++i) {
				spawn_item_on_ground(ai.target_x, ai.target_y, ai.target_z, "wood_log");
			}

			// Update pathing
			for (int Z=-2; Z<10; ++Z) {
				for (int Y=-10; Y<10; ++Y) {
					for (int X=-10; X<10; ++X) {
						current_region->determine_tile_standability(pos.x + X, pos.y + Y, pos.z + Z);
						current_region->determine_tile_connectivity(pos.x + X, pos.y + Y, pos.z + Z);
					}
				}
			}

			// Change status to drop axe
			emit(map_dirty_message{});
			emit(inventory_changed_message{});
			ai.job_type_minor = JM_DROP_AXE;
			change_job_status(ai, name, "Dropping axe");
		} else if (skill_check == CRITICAL_FAIL) {
			// Damage yourself
		}
		return;
	}

	if (ai.job_type_minor == JM_DROP_AXE) {
		if (ai.current_tool == 0) std::cout << "Warning: axe is unassigned at this time\n";
		drop_current_tool(e, ai, pos);
		become_idle(e, ai, name);
		return;
	}
}

void settler_ai_system::do_building(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_SELECT_COMPONENT) {
		bool has_components = true;
		for (std::pair<std::size_t, bool> &component : ai.building_target.get().component_ids) {
			if (!component.second) {
				has_components = false;
				ai.current_tool = component.first;
				ai.current_path = find_path(pos, get_item_location(ai.current_tool));
				if (ai.current_path->success) {
					component.second = true;
					ai.job_type_minor = JM_GO_TO_COMPONENT;
					change_job_status(ai, name, "Traveling to building component");
				} else {
					cancel_action(e, ai, stats, species, pos, name, "Component unavailable");
				}
				return;
			}
		}

		if (has_components) {
			ai.job_type_minor = JM_ASSEMBLE;
			change_job_status(ai, name, "Constructing building");
		}
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_COMPONENT) {
		if (pos == ai.current_path->destination) {
			// We're at the component
			ai.current_path.reset();
			ai.job_type_minor = JM_COLLECT_COMPONENT;
			change_job_status(ai, name, "Collect building component");
			return;
		}
		// Travel to component
		position_t next_step = ai.current_path->steps.front();
		move_to(e, pos, next_step);
		ai.current_path->steps.pop_front();
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_COMPONENT) {
		// Find the pick, remove any position or stored components, add a carried_by component
		try { delete_component<position_t>(ai.current_tool); } catch (...) {}
		try { delete_component<item_stored_t>(ai.current_tool); } catch (...) {}
		entity(ai.current_tool)->assign(item_carried_t{ INVENTORY, e.id });

		// Notify the inventory system of the change	
		emit(inventory_changed_message{});
		emit(renderables_changed_message{});

		ai.job_type_minor = JM_GO_TO_BUILDING;
		change_job_status(ai, name, "Going to building site");
		ai.current_path = find_path(pos, position_t{ai.building_target.get().x, ai.building_target.get().y, ai.building_target.get().z});
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_BUILDING) {
		if (pos == ai.current_path->destination) {
			// We're at the site
			ai.current_path.reset();
			ai.job_type_minor = JM_DROP_COMPONENT;
			change_job_status(ai, name, "Dropping building component");
			return;
		}
		// Travel to site
		position_t next_step = ai.current_path->steps.front();
		move_to(e, pos, next_step);
		ai.current_path->steps.pop_front();
		return;
	}

	if (ai.job_type_minor == JM_DROP_COMPONENT) {
		if (ai.current_tool == 0) std::cout << "Warning: component is unassigned at this time\n";
		drop_current_tool(e, ai, pos);
		ai.current_tool = 0;
		ai.job_type_minor = JM_SELECT_COMPONENT;
		change_job_status(ai, name, "Reading building plans");
		return;
	}

	if (ai.job_type_minor == JM_ASSEMBLE) {
		std::string tag = ai.building_target.get().tag;
		auto finder = building_defs.find(tag);
		if (finder == building_defs.end()) throw std::runtime_error("Building tag unknown!");

		// Make a skill roll
		const std::string skill = finder->second.skill.first;
		const int difficulty = finder->second.skill.second;
		auto skill_check = skill_roll(stats, rng, skill, difficulty);

		if (skill_check >= SUCCESS) {
			// Destroy components
			for (auto &comp : ai.building_target.get().component_ids) {
				delete_item(comp.first);
			}

			// Place the building
			entity(ai.building_target.get().building_entity)->component<building_t>()->complete = true;
			emit(renderables_changed_message{});
			emit(inventory_changed_message{});
			emit(update_workflow_message{});

			// Become idle
			become_idle(e, ai, name);
		}
	}
}

void settler_ai_system::do_reaction(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_SELECT_INPUT) {
		bool has_components = true;
		for (std::pair<std::size_t, bool> &component : ai.reaction_target.get().components) {
			if (!component.second) {
				has_components = false;
				ai.current_tool = component.first;
				ai.current_path = find_path(pos, get_item_location(ai.current_tool));
				if (ai.current_path->success) {
					component.second = true;
					ai.job_type_minor = JM_GO_TO_INPUT;
					change_job_status(ai, name, ai.reaction_target.get().job_name + std::string(" (Travel)"));
				} else {
					cancel_action(e, ai, stats, species, pos, name, "Component unavailable");
				}
				return;
			}
		}

		if (has_components) {
			ai.job_type_minor = JM_REACT;
			change_job_status(ai, name, ai.reaction_target.get().job_name);
		}
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_INPUT) {
		if (pos == ai.current_path->destination) {
			// We're at the component
			ai.current_path.reset();
			ai.job_type_minor = JM_COLLECT_INPUT;
			change_job_status(ai, name, ai.reaction_target.get().job_name + std::string(" (Collect)"));
			return;
		}
		// Travel to component
		position_t next_step = ai.current_path->steps.front();
		move_to(e, pos, next_step);
		ai.current_path->steps.pop_front();
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_INPUT) {
		// Find the pick, remove any position or stored components, add a carried_by component
		try { delete_component<position_t>(ai.current_tool); } catch (...) {}
		try { delete_component<item_stored_t>(ai.current_tool); } catch (...) {}
		entity(ai.current_tool)->assign(item_carried_t{ INVENTORY, e.id });

		// Notify the inventory system of the change	
		emit(inventory_changed_message{});
		emit(renderables_changed_message{});

		ai.job_type_minor = JM_GO_TO_WORKSHOP;
		change_job_status(ai, name, ai.reaction_target.get().job_name + std::string(" (Travel)"));
		position_t * reactor_pos = entity(ai.reaction_target.get().building_id)->component<position_t>();
		ai.current_path = find_path(pos, position_t{reactor_pos->x, reactor_pos->y, reactor_pos->z});
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_WORKSHOP) {
		if (pos == ai.current_path->destination) {
			// We're at the site
			ai.current_path.reset();
			ai.job_type_minor = JM_DROP_INPUT;
			change_job_status(ai, name, ai.reaction_target.get().job_name + std::string(" (Drop)"));
			return;
		}
		// Travel to site
		position_t next_step = ai.current_path->steps.front();
		move_to(e, pos, next_step);
		ai.current_path->steps.pop_front();
		return;
	}

	if (ai.job_type_minor == JM_DROP_INPUT) {
		if (ai.current_tool == 0) std::cout << "Warning: component is unassigned at this time\n";
		drop_current_tool(e, ai, pos);
		ai.current_tool = 0;
		ai.job_type_minor = JM_SELECT_INPUT;
		change_job_status(ai, name, ai.reaction_target.get().job_name + std::string(" (Planning)"));
		return;
	}

	if (ai.job_type_minor == JM_REACT) {
		// Skill check, destroy inputs, create outputs
		auto finder = reaction_defs.find(ai.reaction_target.get().reaction_tag);
		auto skill_check = skill_roll(stats, rng, finder->second.skill, finder->second.difficulty);

		if (skill_check >= SUCCESS) {
			// Delete components
			for (auto comp : ai.reaction_target.get().components) {
				delete_item(comp.first);
			}

			// Spawn results
			for (auto &output : finder->second.outputs) {
				for (int i=0; i<output.second; ++i) {
					spawn_item_on_ground(pos.x, pos.y, pos.z, output.first);
				}
			}

			// Finish
			free_workshop(ai.reaction_target.get().building_id);
			emit(renderables_changed_message{});
			emit(inventory_changed_message{});

			// Become idle
			become_idle(e, ai, name);
		}
	}
}