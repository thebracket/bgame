#pragma once

#include <rltk.hpp>

using namespace rltk;

struct construct_door_t {
    std::size_t civ_owner = 0;
	bool locked = false;

	construct_door_t() {}
	construct_door_t(const int &owner) : civ_owner(owner) {}

	std::size_t serialization_identity = 29;

	void save(std::ostream &lbfile);
	static construct_door_t load(std::istream &lbfile);
};