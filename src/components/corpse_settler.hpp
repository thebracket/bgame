#pragma once

#include <rltk.hpp>

using namespace rltk;

struct corpse_settler {
	corpse_settler() {}
	corpse_settler(const std::string cause) : cause_of_death(cause) {}
    uint32_t ticks_since_death = 0;
	std::string cause_of_death = "";

	std::size_t serialization_identity = 22;

	void save(std::ostream &lbfile) {
        serialize(lbfile, ticks_since_death);
		serialize(lbfile, cause_of_death);
	}

	static corpse_settler load(std::istream &lbfile) {
		corpse_settler c;
        deserialize(lbfile, c.ticks_since_death);
		deserialize(lbfile, c.cause_of_death);
		return c;
	}
};