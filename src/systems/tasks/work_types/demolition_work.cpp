#include "demolition_work.hpp"
#include "../../../messages/messages.hpp"
#include "../../ai/path_finding.hpp"
#include "../../ai/inventory_system.hpp"
#include "../idle_mode.hpp"
#include "../settler_glyph.hpp"
#include "../settler_job_status.hpp"
#include "../settler_drop_tool.hpp"
#include "../settler_cancel_action.hpp"
#include "../pathfinding.hpp"
#include "../initiative.hpp"
#include "../../../raws/buildings.hpp"
#include "../../../planet/region.hpp"

using namespace rltk;
using namespace region;
using tasks::become_idle;
using tasks::change_settler_glyph;
using tasks::change_job_status;
using tasks::drop_current_tool;
using tasks::cancel_action;
using tasks::follow_path;
using tasks::follow_result_t;
using tasks::calculate_initiative;


void do_deconstruction(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_DECONSTRUCT) {
		std::cout << "find\n";
		auto target_building = entity(ai.target_id);
		if (!target_building) {
			cancel_action(e, ai, stats, species, pos, name, "No route to deconstruction");
			return;
		}
		auto target_pos = target_building->component<position_t>();
		if (!target_pos) {
			cancel_action(e, ai, stats, species, pos, name, "No route to deconstruction");
			return;
		}
		ai.target_x = target_pos->x;
		ai.target_y = target_pos->y;
		ai.target_z = target_pos->z;
		ai.current_path = find_path(pos, *target_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to deconstruction");
			return;
		}
		ai.job_type_minor = JM_GO_TO_DECONSTRUCT;
		change_job_status(ai, name, "Traveling to demolition.");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_DECONSTRUCT) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_DECONSTRUCT;
				change_job_status(ai, name, "Performing demolition");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				cancel_action(e, ai, stats, species, pos, name, "No route to deconstruction");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_DECONSTRUCT) {
		// Make a skill roll
		const std::string skill = "Construction";
		const int difficulty = 10;
		auto skill_check = skill_roll(e.id, stats, rng, skill, difficulty);
		if (skill_check >= SUCCESS) {
			auto building_entity = entity(ai.target_id);
			if (building_entity) {
				auto building_comp = building_entity->component<building_t>();
				auto building_pos = building_entity->component<position_t>();
				if (building_comp && building_pos) {
					auto finder = building_defs.find(building_comp->tag);
					if (finder != building_defs.end()) {
						for (const std::pair<std::string, std::size_t> &component : building_comp->built_with) {
							spawn_item_on_ground(building_pos->x, building_pos->y, building_pos->z, component.first, component.second);
						}
						delete_entity(ai.target_id);
						ai.target_id = 0;
					}
				}
				emit_deferred(door_changed_message{});
				// Update pathing
				for (int i=0; i<2; ++i) {
					for (int Z=-2; Z<4; ++Z) {
						for (int Y=-10; Y<10; ++Y) {
							for (int X=-10; X<10; ++X) {
								tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
							}
						}
					}
				}
			}
			become_idle(e, ai, name);
			emit(opacity_changed_message{});
			return;
		} else {
			// Failed!
			if (skill_check == CRITICAL_FAIL) emit_deferred(inflict_damage_message{e.id, 1, "Demolition Accident"});
			return;
		}
	}	
}

void do_demolition(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_DEMOLISH) {
		std::tie(ai.target_x, ai.target_y, ai.target_z) = idxmap(ai.target_id);

		int x = ai.target_x;
		int y = ai.target_y;
		int z = ai.target_z;

		if (flag(mapidx(x-1,y,z), CAN_STAND_HERE)) {
			x=x-1;
		} else if (flag(mapidx(x+1,y,z), CAN_STAND_HERE)) {
			x=x+1;
		} else if (flag(mapidx(x,y-1,z), CAN_STAND_HERE)) {
			y=y-1;
		} else if (flag(mapidx(x,y+1,z), CAN_STAND_HERE)) {
			y=y+1;
		} else if (flag(mapidx(x,y,z-1), CAN_STAND_HERE)) {
			z=z-1;
		} else if (flag(mapidx(x,y,z+1), CAN_STAND_HERE)) {
			z=z+1;
		} else {
			cancel_action(e, ai, stats, species, pos, name, "No route to demolition");
			return;
		}

		ai.current_path = find_path(pos, position_t{x, y, z});
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to demolition");
			return;
		}
		ai.job_type_minor = JM_GO_TO_DEMOLISH;
		change_job_status(ai, name, "Traveling to demolition.");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_DEMOLISH) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_DEMOLISH;
				change_job_status(ai, name, "Performing demolition");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				cancel_action(e, ai, stats, species, pos, name, "No route to deconstruction");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_DEMOLISH) {
		// Make a skill roll
		const std::string skill = "Construction";
		const int difficulty = 15;
		auto skill_check = skill_roll(e.id, stats, rng, skill, difficulty);
		if (skill_check >= SUCCESS) {
            make_open_space(ai.target_id);
			spawn_item_on_ground(pos.x, pos.y, pos.z, "block", material(ai.target_id));
            reveal(ai.target_id);
            reveal(mapidx(ai.target_x, ai.target_y, ai.target_z-1));
			become_idle(e, ai, name);
			// Update pathing
			for (int i=0; i<2; ++i) {
				for (int Z=-2; Z<4; ++Z) {
					for (int Y=-10; Y<10; ++Y) {
						for (int X=-10; X<10; ++X) {
							tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
						}
					}
				}
			}
			emit_deferred(tile_removed_message{ai.target_x, ai.target_y, ai.target_z});
			emit_deferred(door_changed_message{});
			
			return;
		} else {
			// Failed!
			if (skill_check == CRITICAL_FAIL) emit_deferred(inflict_damage_message{e.id, 1, "Demolition Accident"});
			return;
		}
	}
}
