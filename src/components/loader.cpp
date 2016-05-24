#include "loader.hpp"
#include "components.hpp"
#include <rltk.hpp>
#include <iostream>

void component_loader(std::istream &lbfile, std::size_t type_id, std::size_t entity_id) {
	switch (type_id) {
		case 1 : entity(entity_id)->assign(world_position_t::load(lbfile)); break;
		case 2 : entity(entity_id)->assign(position_t::load(lbfile)); break;
		case 3 : entity(entity_id)->assign(calendar_t::load(lbfile)); break;
		case 4 : entity(entity_id)->assign(renderable_t::load(lbfile)); break;
		case 5 : entity(entity_id)->assign(name_t::load(lbfile)); break;
		case 6 : entity(entity_id)->assign(species_t::load(lbfile)); break;
		case 7 : entity(entity_id)->assign(health_t::load(lbfile)); break;
		case 8 : entity(entity_id)->assign(game_stats_t::load(lbfile)); break;
		case 9 : entity(entity_id)->assign(item_t::load(lbfile)); break;
		case 10 : entity(entity_id)->assign(item_carried_t::load(lbfile)); break;
	}
}
