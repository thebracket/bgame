#include "settler_leisure.hpp"
#include "settler_cancel_action.hpp"
#include "settler_drop_tool.hpp"
#include "settler_cancel_action.hpp"
#include "settler_wander.hpp"

using tasks::cancel_action;
using tasks::drop_current_tool;

void do_leisure_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.current_tool != 0) drop_current_tool(entity, ai, pos);
	if (ai.job_type_major == JOB_SLEEP) {
		cancel_action(entity, ai, stats, species, pos, name, "Time to wake up");
		return;
	}
	wander_randomly(entity, pos);
	if (ai.job_status != "Idle") ai.job_status = "Idle";
}