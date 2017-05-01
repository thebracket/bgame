#include "idle_mode.hpp"
#include "../../components/corpse_harvestable.hpp"
#include "settler_glyph.hpp"
#include "settler_drop_tool.hpp"
#include "settler_job_status.hpp"

namespace tasks {

void become_idle(entity_t &e, settler_ai_t &ai, name_t &name) {

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