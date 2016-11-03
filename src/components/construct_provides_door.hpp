#pragma once

#include <rltk.hpp>

using namespace rltk;

struct construct_door_t {
	bool locked = false;

	construct_door_t() {}

	std::size_t serialization_identity = 29;

	void save(std::ostream &lbfile);
	static construct_door_t load(std::istream &lbfile);
};