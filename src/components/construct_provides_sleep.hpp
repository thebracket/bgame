#pragma once

#include <rltk.hpp>

using namespace rltk;

struct construct_provides_sleep_t {
	bool claimed = false;

	construct_provides_sleep_t() {}

	std::size_t serialization_identity = 16;

	void save(std::ostream &lbfile) {
		serialize(lbfile, claimed);
	}

	static construct_provides_sleep_t load(std::istream &lbfile) {
		construct_provides_sleep_t c;
		deserialize(lbfile, c.claimed);
		return c;
	}
};