#pragma once

#include <rltk.hpp>

using namespace rltk;

struct wildlife_group {
	wildlife_group() {}
	wildlife_group(uint8_t &id) : group_id(id) {}
    uint8_t group_id;

	std::size_t serialization_identity = 21;

	void save(std::ostream &lbfile);
	static wildlife_group load(std::istream &lbfile);
};