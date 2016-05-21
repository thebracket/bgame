#pragma once

#include <rltk.hpp>

using namespace rltk;

struct health_t {
	int max_hitpoints;
	int current_hitpoints;

	health_t() {}

	std::size_t serialization_identity = 7;

	void save(std::ostream &lbfile) {
		serialize(lbfile, max_hitpoints);
		serialize(lbfile, current_hitpoints);
	}

	static health_t load(std::istream &lbfile) {
		health_t c;
		deserialize(lbfile, c.max_hitpoints);
		deserialize(lbfile, c.current_hitpoints);
		return c;
	}
};