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
		case 11 : entity(entity_id)->assign(settler_ai_t::load(lbfile)); break;
		case 12 : entity(entity_id)->assign(item_stored_t::load(lbfile)); break;
		case 13 : entity(entity_id)->assign(designations_t::load(lbfile)); break;
		case 14 : entity(entity_id)->assign(construct_container_t::load(lbfile)); break;
		case 15 : entity(entity_id)->assign(construct_power_t::load(lbfile)); break;
		case 16 : entity(entity_id)->assign(construct_provides_sleep_t::load(lbfile)); break;
		case 17  : entity(entity_id)->assign(viewshed_t::load(lbfile)); break;
		case 18  : entity(entity_id)->assign(building_t::load(lbfile)); break;
		case 19  : entity(entity_id)->assign(lightsource_t::load(lbfile)); break;
		case 20  : entity(entity_id)->assign(grazer_ai::load(lbfile)); break;
		case 21  : entity(entity_id)->assign(wildlife_group::load(lbfile)); break;
		case 22  : entity(entity_id)->assign(corpse_settler::load(lbfile)); break;
		case 23  : entity(entity_id)->assign(corpse_harvestable::load(lbfile)); break;
		case 24  : entity(entity_id)->assign(smoke_emitter_t::load(lbfile)); break;
		case 25  : entity(entity_id)->assign(sentient_ai::load(lbfile)); break;
		case 26  : entity(entity_id)->assign(logger_t::load(lbfile)); break;
		default : throw std::runtime_error("Unknown component type, "+std::to_string(type_id));
	}
}
