#include "settler_cancel_action.hpp"
#include "../../components/construct_provides_sleep.hpp"
#include "../../main/game_globals.hpp"
#include "settler_drop_tool.hpp"
#include "idle_mode.hpp"

namespace tasks {

void cancel_action(rltk::entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name, const std::string reason) {
	// Drop whatever we are doing!
	if (ai.job_type_major == JOB_SLEEP) {
		auto bed_entity = entity(ai.target_id);
		if (bed_entity) {
			auto bed = bed_entity->component<construct_provides_sleep_t>();
			if (bed) bed->claimed = false;
		}
	}
	if (ai.job_type_major == JOB_GUARD) {
		const int idx = mapidx(ai.target_x, ai.target_y, ai.target_z);
		for (auto &g : designations->guard_points) {
			if (mapidx(g.second) == idx) g.first = false;
		}
	}
	// Drop tool
	drop_current_tool(e, ai, pos);

	std::cout << name.first_name << " cancels action: " << reason << "\n";
	become_idle(e, ai, name);
}

}