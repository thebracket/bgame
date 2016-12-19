#include "guard_duty.hpp"
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

void do_guard_duty(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_GUARDPOST) {
		position_t guard_pos = position_t{ai.target_x, ai.target_y, ai.target_z};
		ai.current_path = find_path(pos, guard_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to guard post");
			return;
		}
		ai.job_type_minor = JM_GO_TO_GUARDPOST;
		change_job_status(ai, name, "Traveling to guard post.");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_GUARDPOST) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_GUARD;
				change_job_status(ai, name, "Guarding");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				cancel_action(e, ai, stats, species, pos, name, "No route to guardpost");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_GUARD) {
		if (shooting_range(e, pos) == 0) cancel_action(e, ai, stats, species, pos, name, "No ammunition");
	}
}
