#include "settler_ai_system.hpp"
#include "../messages/messages.hpp"
#include "../components/components.hpp"
#include "../game_globals.hpp"
#include <iostream>

using namespace rltk;

void settler_ai_system::update(const double duration_ms) {
}

inline void settler_calculate_initiative(settler_ai_t &ai, game_stats_t &stats) {
	ai.initiative = rng.roll_dice(1, 6) - stat_modifier(stats.dexterity);
}

void settler_ai_system::configure() {
	subscribe<tick_message>([this](tick_message &msg) {
		each<settler_ai_t, game_stats_t, species_t, position_t, name_t>([] (entity_t &entity, settler_ai_t &ai, game_stats_t &stats, 
			species_t &species, position_t &pos, name_t &name) 
		{
			if (ai.initiative < 1) {
				//std::cout << name.first_name << " " << name.last_name << " (" << stats.profession_tag << ") is thinking.\n";

				settler_calculate_initiative(ai, stats);
			} else {
				--ai.initiative;
			}
		});
	});
}
