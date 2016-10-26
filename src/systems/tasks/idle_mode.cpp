#include "idle_mode.hpp"
#include "../../components/corpse_harvestable.hpp"
#include "../../main/game_globals.hpp"
#include "settler_glyph.hpp"
#include "settler_drop_tool.hpp"
#include "settler_job_status.hpp"

namespace tasks {

void become_idle(entity_t &e, settler_ai_t &ai, name_t &name) {
	if (ai.job_type_major == JOB_BUTCHERING && ai.targeted_hostile>0) {
		auto body = entity(ai.targeted_hostile);
		if (body) {
			auto corpse = body->component<corpse_harvestable>();
			if (corpse) corpse->claimed = false;
		}
	}

	if (ai.job_type_major == JOB_GUARD) {
		const auto idx = mapidx(ai.target_x, ai.target_y, ai.target_z);
		for (auto &g : designations->guard_points) {
			if (mapidx(g.second) == idx) g.first = false;
		}
	}

	ai.job_type_major = JOB_IDLE;
	ai.target_x = 0; 
	ai.target_y = 0; 
	ai.target_z = 0;
	change_settler_glyph(e, vchar{1, rltk::colors::YELLOW, rltk::colors::BLACK});
	change_job_status(ai, name, "Idle");
	if (ai.current_tool != 0) {
		auto pos = e.component<position_t>();
		drop_current_tool(e, ai, *pos);
	}
}

}