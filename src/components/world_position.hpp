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

	void save(std::ostream &lbfile) {
		serialize(lbfile, world_x);
		serialize(lbfile, world_y);
		serialize(lbfile, region_x);
		serialize(lbfile, region_y);
		serialize(lbfile, region_z);
	}
};