#pragma once

#include <rltk.hpp>

using namespace rltk;

struct corpse_harvestable {
	corpse_harvestable() {}
    corpse_harvestable(const std::string &tag) : creature_tag(tag) {}
    uint32_t ticks_since_death = 0;
    std::string creature_tag = "";
	bool claimed = false;

	std::size_t serialization_identity = 23;

	void save(std::ostream &lbfile) {
        serialize(lbfile, ticks_since_death);
        serialize(lbfile, creature_tag);
	}

	static corpse_harvestable load(std::istream &lbfile) {
		corpse_harvestable c;
        deserialize(lbfile, c.ticks_since_death);
        deserialize(lbfile, c.creature_tag);
		return c;
	}
};