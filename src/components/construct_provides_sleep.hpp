#pragma once

#include <rltk.hpp>

using namespace rltk;

struct construct_provides_sleep_t {
	bool claimed = false;

	construct_provides_sleep_t() {}

	std::size_t serialization_identity = 16;

	void save(std::ostream &lbfile);
	static construct_provides_sleep_t load(std::istream &lbfile);
};