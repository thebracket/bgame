#include "mining_work.hpp"
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
#include "../../../utils/telemetry.hpp"

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

void do_mining(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {

	if (ai.job_type_minor == JM_FIND_PICK) {
		auto pick = claim_closest_item_by_category(TOOL_DIGGING, pos);
		if (pick==0) {
			cancel_action(e, ai, stats, species, pos, name, "No available pick");
			return;
		}
		auto pick_pos = get_item_location(pick);
		if (!pick_pos) {
			cancel_action(e, ai, stats, species, pos, name, "No available pick");
			return;
		}
		ai.current_path = find_path(pos, *pick_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No available pick");
			return;
		}
		ai.job_type_minor = JM_GO_TO_PICK;
		ai.target_x = pick_pos->x;
		ai.target_y = pick_pos->y;
		ai.target_z = pick_pos->z;
		change_job_status(ai, name, "Traveling to digging tool");
		ai.current_tool = pick;
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_PICK) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_PICK;
				change_job_status(ai, name, "Collect digging tool");
			}, // On arrival
			[&ai, &name, &e, &stats, &species, &pos] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No available pick");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_PICK) {
		// Find the pick, remove any position or stored components, add a carried_by component
		emit(pickup_item_message{ai.current_tool, e.id});

		ai.job_type_minor = JM_GO_TO_SITE;
		change_job_status(ai, name, "Travelling to dig site");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_SITE) {
		const auto idx = mapidx(pos.x, pos.y, pos.z);
		if (mining_map[idx]==0) {
			// We're at a diggable site
			ai.job_type_minor = JM_DIG;
			change_job_status(ai, name, "Digging");
			return;
		}
		// Look at adjacent mining map entries, and path to the closest one. If there is no option,
		// Drop the pick.
		int current_direction = 0;
		uint8_t min_value = std::numeric_limits<uint8_t>::max();
		if (mining_map[mapidx(pos.x, pos.y-1, pos.z)] < min_value && current_region->tile_flags[idx].test(CAN_GO_NORTH)) { 
			min_value = mining_map[mapidx(pos.x, pos.y-1, pos.z)]; 
			current_direction = 1; 
		}
		if (mining_map[mapidx(pos.x, pos.y+1, pos.z)] < min_value && current_region->tile_flags[idx].test(CAN_GO_SOUTH)) { 
			min_value = mining_map[mapidx(pos.x, pos.y+1, pos.z)]; 
			current_direction = 2; 
		}
		if (mining_map[mapidx(pos.x-1, pos.y, pos.z)] < min_value && current_region->tile_flags[idx].test(CAN_GO_WEST)) { 
			min_value = mining_map[mapidx(pos.x-1, pos.y, pos.z)]; 
			current_direction = 3; 
		}
		if (mining_map[mapidx(pos.x+1, pos.y, pos.z)] < min_value && current_region->tile_flags[idx].test(CAN_GO_EAST)) { 
			min_value = mining_map[mapidx(pos.x+1, pos.y, pos.z)]; 
			current_direction = 4; 
		}
		if (mining_map[mapidx(pos.x, pos.y, pos.z-1)] < min_value && current_region->tile_flags[idx].test(CAN_GO_DOWN)) { 
			min_value = mining_map[mapidx(pos.x, pos.y, pos.z-1)]; 
			current_direction = 5; 
		}
		if (mining_map[mapidx(pos.x, pos.y, pos.z+1)] < min_value && current_region->tile_flags[idx].test(CAN_GO_UP)) { 
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
		auto skill_check = skill_roll(e.id, stats, rng, "Mining", DIFICULTY_TOUGH);
		if (skill_check >= SUCCESS) {
			// Determine the digging target from here
			// Make a skill roll, and if successful complete the action
			// When complete, move to dropping the pick
			const auto idx = mapidx(pos.x, pos.y, pos.z);
			const int target_idx = mining_targets[idx];
			const int target_operation = designations->mining[target_idx];
            call_home("mining", std::to_string(target_operation));

			if (target_operation > 0) {
				emit(perform_mining_message{mining_targets[idx], designations->mining[target_idx], static_cast<int>(pos.x), static_cast<int>(pos.y), pos.z});
				designations->mining.erase(target_idx);
			}
			if (designations->mining.empty()) {
				ai.job_type_minor = JM_DROP_PICK;
			} else {
				ai.job_type_minor = JM_GO_TO_SITE;
			}
			change_job_status(ai, name, "Dropping digging tools - work complete");
			return;
		} else {
			// Failed!
			if (skill_check == CRITICAL_FAIL) emit_deferred(inflict_damage_message{e.id, 1, "Mining Accident"});
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

