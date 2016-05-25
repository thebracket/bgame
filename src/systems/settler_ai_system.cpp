#include "settler_ai_system.hpp"
#include "../messages/messages.hpp"
#include "../components/components.hpp"
#include <iostream>

using namespace rltk;

void settler_ai_system::update(const double duration_ms) {
}

void settler_ai_system::configure() {
	subscribe<tick_message>([this](tick_message &msg) {
		each<settler_ai_t, game_stats_t, species_t, position_t, name_t>([] (entity_t &entity, settler_ai_t &ai, game_stats_t &stats, 
			species_t &species, position_t &pos, name_t &name) 
		{
			std::cout << name.first_name << " is thinking.\n";
		});
	});
}
