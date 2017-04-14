#include "chopping_work.hpp"
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

void do_chopping(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {

	if (ai.job_type_minor == JM_FIND_AXE) {
		auto axe = claim_closest_item_by_category(TOOL_CHOPPING, pos);
		if (axe==0) {
			cancel_action(e, ai, stats, species, pos, name, "No available axe");
			return;
		}
		auto axe_pos = get_item_location(axe);
		if (!axe_pos) {
			cancel_action(e, ai, stats, species, pos, name, "No available axe");
			return;
		}
		ai.current_path = find_path(pos, *axe_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to available axe");
			return;
		}
		ai.job_type_minor = JM_GO_TO_AXE;
		ai.target_x = axe_pos->x;
		ai.target_y = axe_pos->y;
		ai.target_z = axe_pos->z;
		change_job_status(ai, name, "Traveling to chopping tool");
		ai.current_tool = axe;
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_AXE) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_AXE;
				change_job_status(ai, name, "Collect chopping tool");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to available axe");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_AXE) {
		// Find the pick, remove any position or stored components, add a carried_by component
		emit(pickup_item_message{ai.current_tool, e.id});

		ai.job_type_minor = JM_FIND_TREE;
		change_job_status(ai, name, "Pathing to chopping site");
		return;
	}

	if (ai.job_type_minor == JM_FIND_TREE) {
		if (designations == nullptr) std::cout << "Warning - no designations!\n";
        if (designations->chopping.empty()) {
            std::cout << "Warning - chopping designations are empty!\n";
            cancel_action(e, ai, stats, species, pos, name, "No route to available axe");
            return;
        }
		position_t tree_pos = designations->chopping.begin()->second;
		ai.target_id = designations->chopping.begin()->first;

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
			designations->chopping.erase(ai.target_id);
			cancel_action(e, ai, stats, species, pos, name, "Can't find tree");
			return;
		}
	}

	if (ai.job_type_minor == JM_GO_TO_TREE) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_CHOP;
				change_job_status(ai, name, "Chopping tree");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to tree");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_CHOP) {
		// Chop down the tree
		auto skill_check = skill_roll(e.id, stats, rng, "Lumberjacking", DIFICULTY_TOUGH);

		if (skill_check >= SUCCESS) {
			call_home("tree_chop");
			// Tree is going down!
			int number_of_logs = 0;
            int tree_idx = 0;
            int lowest_z = 1000;
			for (int z=0; z<REGION_DEPTH; ++z) {
				for (int y=0; y<REGION_HEIGHT; ++y) {
					for (int x=0; x<REGION_WIDTH; ++x) {
						const auto idx = mapidx(x,y,z);
						if (current_region->tree_id[idx] == ai.target_id) {
                            if (z < lowest_z) {
                                lowest_z = z;
                                tree_idx = idx;
                            }

							current_region->solid[idx]=false;
							current_region->opaque[idx]=false;
							current_region->tile_flags[idx].reset(CAN_STAND_HERE);
							current_region->tree_id[idx] = 0;
							current_region->tile_type[idx] = tile_type::OPEN_SPACE;
							current_region->tile_calculate(x,y,z);
							++number_of_logs;
						}
					}
				}
			}
			current_region->tile_type[tree_idx] = tile_type::FLOOR;
            current_region->tile_flags[tree_idx].set(CAN_STAND_HERE);
			current_region->tile_calculate(ai.target_x, ai.target_y, ai.target_z);
			int tx,ty,tz;
			std::tie(tx,ty,tz) = idxmap(tree_idx);

			// Spawn wooden logs
			number_of_logs = (number_of_logs/20)+1;
			for (int i=0; i<number_of_logs; ++i) {
				spawn_item_on_ground(tx, ty, tz, "wood_log", get_material_by_tag("wood"));
			}

			// Update pathing
			for (int Z=-2; Z<10; ++Z) {
				for (int Y=-10; Y<10; ++Y) {
					for (int X=-10; X<10; ++X) {
						current_region->tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
						current_region->tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
					}
				}
			}

			// Change status to drop axe or continue
			if (designations->chopping.empty()) {
				emit_deferred(map_dirty_message{});
				emit_deferred(inventory_changed_message{});
				ai.job_type_minor = JM_DROP_AXE;
				change_job_status(ai, name, "Dropping axe");
			} else {
				ai.job_type_minor = JM_FIND_TREE;
				change_job_status(ai, name, "Pathing to chopping site");
			}
		} else if (skill_check == CRITICAL_FAIL) {
			// Damage yourself
			emit_deferred(inflict_damage_message{e.id, 1, "Lumberjacking Accident"});
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
