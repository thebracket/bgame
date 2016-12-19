#include "equip_ranged_work.hpp"
#include "../settler_work_time.hpp"
#include "../../../messages/messages.hpp"
#include "../../../main/game_globals.hpp"
#include "../../path_finding.hpp"
#include "../../mining_system.hpp"
#include "../../inventory_system.hpp"
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

void do_equip_ranged(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_RANGED_WEAPON) {
		int max_range = -1;
		if (designations->standing_order_upgrade == standing_orders::SO_UPGRADE_NEARBY) max_range = 15;
		auto axe = claim_closest_item_by_category(WEAPON_RANGED, pos, max_range);
		if (axe==0) {
			cancel_action(e, ai, stats, species, pos, name, "No available ranged weapon");
			return;
		}
		auto axe_pos = get_item_location(axe);
		ai.current_path = find_path(pos, *axe_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to available ranged weapon");
			return;
		}
		ai.job_type_minor = JM_GO_TO_RANGED_WEAPON;
		ai.target_x = axe_pos->x;
		ai.target_y = axe_pos->y;
		ai.target_z = axe_pos->z;
		change_job_status(ai, name, "Traveling to ranged weapon");
		ai.current_tool = axe;
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_RANGED_WEAPON) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_RANGED_WEAPON;
				change_job_status(ai, name, "Collect ranged weapon");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to weapon");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_RANGED_WEAPON) {
		// Find the pick, remove any position or stored components, add a carried_by component
		emit(pickup_item_message{ai.current_tool, e.id, EQUIP_RANGED});
		ai.current_tool = 0;

		become_idle(e, ai, name);
		return;
	}	
}

void do_equip_ammo(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_AMMO) {
		int max_range = -1;
		if (designations->standing_order_upgrade == standing_orders::SO_UPGRADE_NEARBY) max_range = 15;
		auto ranged_status = has_ranged_weapon(e);
		auto axe = claim_closest_ammo(WEAPON_AMMO, pos, ranged_status.second, max_range);
		if (axe==0) {
			cancel_action(e, ai, stats, species, pos, name, "No available ammo");
			return;
		}
		auto axe_pos = get_item_location(axe);
		ai.current_path = find_path(pos, *axe_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to available ammo");
			return;
		}
		ai.job_type_minor = JM_GO_TO_AMMO;
		ai.target_x = axe_pos->x;
		ai.target_y = axe_pos->y;
		ai.target_z = axe_pos->z;
		change_job_status(ai, name, "Traveling to ammo");
		ai.current_tool = axe;
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_AMMO) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_AMMO;
				change_job_status(ai, name, "Collect ammo");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to weapon");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_AMMO) {
		// Find the pick, remove any position or stored components, add a carried_by component
		emit(pickup_item_message{ai.current_tool, e.id, EQUIP_AMMO});
		ai.current_tool = 0;

		become_idle(e, ai, name);
		return;
	}	
}
