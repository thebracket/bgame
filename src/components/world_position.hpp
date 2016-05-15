#pragma once

#include <rltk.hpp>

using namespace rltk;

struct world_position_t {
	int world_x=0;
	int world_y=0;
	int region_x=0;
	int region_y=0;
	int region_z=0;
	std::size_t serialization_identity = 1;

	world_position_t(const int wx, const int wy, const int rx, const int ry, const int rz) :
		world_x(wx), world_y(wy), region_x(rx), region_y(ry), region_z(rz) {}
	world_position_t() {}

	void save(std::ostream &lbfile) {
		serialize(lbfile, world_x);
		serialize(lbfile, world_y);
		serialize(lbfile, region_x);
		serialize(lbfile, region_y);
		serialize(lbfile, region_z);
	}

	static world_position_t load(std::istream &lbfile) {
		world_position_t c;
		deserialize(lbfile, c.world_x);
		deserialize(lbfile, c.world_y);
		deserialize(lbfile, c.region_x);
		deserialize(lbfile, c.region_y);
		deserialize(lbfile, c.region_z);
		return c;
	}
};