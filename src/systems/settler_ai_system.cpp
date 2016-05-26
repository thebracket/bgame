#include "settler_ai_system.hpp"
#include "../messages/messages.hpp"
#include "../game_globals.hpp"
#include <iostream>

using namespace rltk;

void settler_ai_system::update(const double duration_ms) {
}

void settler_ai_system::settler_calculate_initiative(settler_ai_t &ai, game_stats_t &stats) {
	ai.initiative = rng.roll_dice(1, 6) - stat_modifier(stats.dexterity);
}

void settler_ai_system::wander_randomly(position_t &pos) {
	const position_t original = pos;
	const int tile_index = current_region.idx(pos.x, pos.y, pos.z);
	//std::cout << current_region.tiles[tile_index].flags << "\n";
	const int direction = rng.roll_dice(1,10);
	switch (direction) {
		case 1 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_UP)) pos.z++; break;
		case 2 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_DOWN)) pos.z--; break;
		case 3 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_NORTH)) pos.y--; break;
		case 4 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_SOUTH)) pos.y++; break;
		case 5 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_EAST)) pos.x++; break;
		case 6 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_WEST)) pos.x--; break;/*
		case 7 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_NORTH_EAST)) { pos.x++; pos.y--; } break;
		case 8 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_SOUTH_EAST)) { pos.x++; pos.y++; } break;
		case 9 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_NORTH_WEST)) { pos.x--; pos.y--; } break;
		case 10 : if (current_region.tiles[tile_index].flags.test(tile_flags::CAN_GO_SOUTH_WEST)) { pos.x++; pos.y++; } break;*/
	}
	if (current_region.tiles[tile_index].flags.test(tile_flags::SOLID)) pos = original;
	emit(renderables_changed_message{});
}

void settler_ai_system::configure() {
	subscribe<tick_message>([this](tick_message &msg) {
		each<settler_ai_t, game_stats_t, species_t, position_t, name_t>([this] (entity_t &entity, settler_ai_t &ai, game_stats_t &stats, 
			species_t &species, position_t &pos, name_t &name) 
		{
			if (ai.initiative < 1) {
				//std::cout << name.first_name << " " << name.last_name << " (" << stats.profession_tag << ") is thinking.\n";
				this->wander_randomly(pos);
				this->settler_calculate_initiative(ai, stats);
			} else {
				--ai.initiative;
			}
		});
	});
}
