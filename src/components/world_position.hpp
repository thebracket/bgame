#pragma once

#include <rltk.hpp>

using namespace rltk;

struct world_position_t {
	int x=0;
	int y=0;
	std::size_t serialization_identity = 1;

	void save(std::ostream &lbfile) {
		serialize(lbfile, x);
		serialize(lbfile, y);
	}
};