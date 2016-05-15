#pragma once

#include <rltk.hpp>

using namespace rltk;

struct position_t {
	int x=0;
	int y=0;
	int z=0;

	position_t() {}
	position_t(const int X, const int Y, const int Z) : x(X), y(Y), z(Z) {}

	std::size_t serialization_identity = 2;

	void save(std::ostream &lbfile) {
		serialize(lbfile, x);
		serialize(lbfile, y);
		serialize(lbfile, z);
	}

	static position_t load(std::istream &lbfile) {
		position_t c;
		deserialize(lbfile, c.x);
		deserialize(lbfile, c.y);
		deserialize(lbfile, c.z);
		return c;
	}
};