#include "demolition_work.hpp"
#include "../settler_work_time.hpp"
#include "../../../messages/messages.hpp"
#include "../../../main/game_globals.hpp"
#include "../../ai/path_finding.hpp"
#include "../../ai/mining_system.hpp"
#include "../../ai/inventory_system.hpp"
#include "../../ai/workflow_system.hpp"
#include "../../ai/wildlife_population_system.hpp"
#include "../../damage/weapons_helpers.hpp"
#include "../../components/item_carried.hpp"
#include "../idle_mode.hpp"
#include "../settler_glyph.hpp"
#include "../settler_job_status.hpp"
#include "../settler_drop_tool.hpp"
#include "../settler_cancel_action.hpp"
#include "../pathfinding.hpp"
#include "../initiative.hpp"
#include "../../../messages/log_message.hpp"
#include "../../../components/logger.hpp"
#include "../../../components/health.hpp"
#include "../../../components/renderable.hpp"
#include "../../../components/corpse_harvestable.hpp"
#include "../../../components/construct_provides_sleep.hpp"
#include "../../../components/viewshed.hpp"
#include "../../../components/smoke_emitter.hpp"
#include "../../../components/grazer_ai.hpp"
#include "../../../components/sentient_ai.hpp"
#include "../../../components/lightsource.hpp"
#include "../../../components/falling.hpp"
#include "../world_queries.hpp"
#include "../settler_sleep.hpp"
#include "../settler_wander.hpp"
#include "../settler_move_to.hpp"
#include "../../../raws/buildings.hpp"

#include <iostream>
#include <map>

using namespace rltk;
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
								current_region->tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
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

		if (current_region->tile_flags[mapidx(x-1,y,z)].test(CAN_STAND_HERE)) {
			x=x-1;
		} else if (current_region->tile_flags[mapidx(x+1,y,z)].test(CAN_STAND_HERE)) {
			x=x+1;
		} else if (current_region->tile_flags[mapidx(x,y-1,z)].test(CAN_STAND_HERE)) {
			y=y-1;
		} else if (current_region->tile_flags[mapidx(x,y+1,z)].test(CAN_STAND_HERE)) {
			y=y+1;
		} else if (current_region->tile_flags[mapidx(x,y,z-1)].test(CAN_STAND_HERE)) {
			z=z-1;
		} else if (current_region->tile_flags[mapidx(x,y,z+1)].test(CAN_STAND_HERE)) {
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
			current_region->tile_type[ai.target_id] = tile_type::OPEN_SPACE;
			current_region->solid[ai.target_id] = false;
			current_region->tile_flags[ai.target_id].reset(CONSTRUCTION);
			current_region->tile_flags[ai.target_id].reset(CAN_STAND_HERE);
			spawn_item_on_ground(pos.x, pos.y, pos.z, "block", current_region->tile_material[ai.target_id]);
			current_region->revealed[ai.target_id] = true;
			current_region->revealed[mapidx(ai.target_x, ai.target_y, ai.target_z-1)] = true;
			become_idle(e, ai, name);
			// Update pathing
			for (int i=0; i<2; ++i) {
				for (int Z=-2; Z<4; ++Z) {
					for (int Y=-10; Y<10; ++Y) {
						for (int X=-10; X<10; ++X) {
							current_region->tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
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
