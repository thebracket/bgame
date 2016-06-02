#include "settler_ai_system.hpp"
#include "../messages/messages.hpp"
#include "../game_globals.hpp"
#include "path_finding.hpp"
#include <iostream>
#include <map>

using namespace rltk;

void settler_ai_system::update(const double duration_ms) {
}

void settler_ai_system::settler_calculate_initiative(settler_ai_t &ai, game_stats_t &stats) {
	ai.initiative = rng.roll_dice(1, 6) - stat_modifier(stats.dexterity);
}

void settler_ai_system::wander_randomly(entity_t &entity, position_t &pos) {
	const position_t original = pos;
	const int tile_index = current_region.idx(pos.x, pos.y, pos.z);
	//std::cout << current_region.tiles[tile_index].flags << "\n";
	const int direction = rng.roll_dice(1,6);
	switch (direction) {
		case 1 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_UP)) pos.z++; break;
		case 2 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_DOWN)) pos.z--; break;
		case 3 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_NORTH)) pos.y--; break;
		case 4 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_SOUTH)) pos.y++; break;
		case 5 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_EAST)) pos.x++; break;
		case 6 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_WEST)) pos.x--; break;
	}
	if (current_region.tiles[tile_index].flags.test(tile_flags::SOLID)) pos = original;

	renderable_t * render = entity.component<renderable_t>();
	render->foreground = rltk::colors::YELLOW;
	render->glyph = '@';
	emit(renderables_changed_message{});
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

void settler_ai_system::cancel_action(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name, const std::string reason) {
	// Drop whatever we are doing!
	if (ai.job_type_major == JOB_SLEEP) {
		each<construct_provides_sleep_t, position_t>([&pos] (entity_t &e, construct_provides_sleep_t &bed, position_t &bed_pos) {
			if (bed_pos == pos) bed.claimed = false;
		});
	}

	std::cout << name.first_name << " cancels action: " << reason << "\n";
	ai.job_type_major = JOB_IDLE;
	ai.target_x = 0; 
	ai.target_y = 0; 
	ai.target_z = 0;
}

void settler_ai_system::do_sleep_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_major != JOB_IDLE && ai.job_type_major != JOB_SLEEP) {
		cancel_action(entity, ai, stats, species, pos, name, "Bed time!");
		return;
	}
	if (ai.job_type_major == JOB_IDLE) {
		ai.job_type_major = JOB_SLEEP;
		ai.job_type_minor = JM_FIND_BED;
		ai.job_status = "Looking for a bed";
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
		ai.job_status = "Going to bed";
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_BED) {
		if (!ai.current_path) {
			ai.job_type_minor = JM_FIND_BED;
			ai.job_status = "Looking for a bed";
			std::cout << "No path to bed - trying again to find one.\n";
			return;
		}
		if (pos == ai.current_path->destination) {
			ai.job_type_minor = JM_SLEEP;
			ai.job_status = "Sleeping";
			return;
		}
		position_t next_step = ai.current_path->steps.front();
		pos.x = next_step.x;
		pos.y = next_step.y;
		pos.z = next_step.z;
		ai.current_path->steps.pop_front();
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
	}
	wander_randomly(entity, pos);
}

void settler_ai_system::do_work_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_major == JOB_SLEEP) {
		cancel_action(entity, ai, stats, species, pos, name, "Time to wake up");
	}
	wander_randomly(entity, pos);
}
