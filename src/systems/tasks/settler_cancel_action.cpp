#include "settler_cancel_action.hpp"
#include "../../components/construct_provides_sleep.hpp"
#include "../../components/claimed_t.hpp"
#include "../../main/game_globals.hpp"
#include "settler_drop_tool.hpp"
#include "idle_mode.hpp"

namespace tasks {

void cancel_action(rltk::entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name, const std::string reason) {
	// Drop whatever we are doing!
	// Drop tool
	drop_current_tool(e, ai, pos);

	std::cout << name.first_name << " cancels action: " << reason << "\n";
	become_idle(e, ai, name);
}

}