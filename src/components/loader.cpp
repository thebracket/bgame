#include "loader.hpp"
#include "components.hpp"
#include <rltk.hpp>

void component_loader(std::istream &lbfile, std::size_t type_id, std::size_t entity_id) {
	switch (type_id) {
		case 1 : entity(entity_id)->assign(world_position_t::load(lbfile)); break;
		case 2 : entity(entity_id)->assign(position_t::load(lbfile)); break;
		case 3 : entity(entity_id)->assign(calendar_t::load(lbfile)); break;
	}
}
