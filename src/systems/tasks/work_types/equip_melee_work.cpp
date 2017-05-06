#include "equip_melee_work.hpp"
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
#include "../../main/game_designations.hpp"

using namespace rltk;
using tasks::become_idle;
using tasks::change_settler_glyph;
using tasks::change_job_status;
using tasks::drop_current_tool;
using tasks::cancel_action;
using tasks::follow_path;
using tasks::follow_result_t;
using tasks::calculate_initiative;

void do_equip_melee(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_MELEE_WEAPON) {
		int max_range = -1;
		if (designations->standing_order_upgrade == standing_orders::SO_UPGRADE_NEARBY) max_range = 15;
		auto axe = claim_closest_item_by_category(WEAPON_MELEE, pos, max_range);
		if (axe==0) {
			cancel_action(e, ai, stats, species, pos, name, "No available melee weapon");
			return;
		}
		auto axe_pos = get_item_location(axe);
		ai.current_path = find_path(pos, *axe_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to available melee weapon");
			return;
		}
		ai.job_type_minor = JM_GO_TO_MELEE_WEAPON;
		ai.target_x = axe_pos->x;
		ai.target_y = axe_pos->y;
		ai.target_z = axe_pos->z;
		change_job_status(ai, name, "Traveling to melee weapon");
		ai.current_tool = axe;
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_MELEE_WEAPON) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_MELEE_WEAPON;
				change_job_status(ai, name, "Collect melee weapon");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to weapon");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_MELEE_WEAPON) {
		// Find the pick, remove any position or stored components, add a carried_by component
		emit(pickup_item_message{ai.current_tool, e.id, EQUIP_MELEE});
		ai.current_tool = 0;

		become_idle(e, ai, name);
		return;
	}	
}
