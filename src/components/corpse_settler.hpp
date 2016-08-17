#pragma once

#include <rltk.hpp>

using namespace rltk;

struct corpse_settler {
	corpse_settler() {}
    uint32_t ticks_since_death = 0;

	std::size_t serialization_identity = 22;

	void save(std::ostream &lbfile) {
        serialize(lbfile, ticks_since_death);
	}

	static corpse_settler load(std::istream &lbfile) {
		corpse_settler c;
        deserialize(lbfile, c.ticks_since_death);
		return c;
	}
};