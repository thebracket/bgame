#pragma once

#include <rltk.hpp>

using namespace rltk;

struct position_t {
	int x=0;
	int y=0;
	int z=0;
	std::size_t serialization_identity = 2;

	void save(std::ostream &lbfile) {
		serialize(lbfile, x);
		serialize(lbfile, y);
		serialize(lbfile, z);
	}
};