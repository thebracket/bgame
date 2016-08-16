#pragma once

#include <rltk.hpp>
#include <string>

using namespace rltk;

enum item_location_t { INVENTORY, HEAD, TORSO, LEGS, FEET, EQUIP_TOOL, EQUIP_MELEE, EQUIP_RANGED, EQUIP_AMMO, EQUIP_SHIELD };

struct item_carried_t {
	item_location_t location = INVENTORY;
	std::size_t carried_by;
	item_carried_t() {}
	item_carried_t(const item_location_t loc, const std::size_t carrier) : location(loc), carried_by(carrier) {}

	std::size_t serialization_identity = 10;

	void save(std::ostream &lbfile) {
		serialize(lbfile, location);
		serialize(lbfile, carried_by);
	}

	static item_carried_t load(std::istream &lbfile) {
		item_carried_t c;
		deserialize(lbfile, c.location);
		deserialize(lbfile, c.carried_by);
		return c;
	}
};