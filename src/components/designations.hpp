#pragma once

#include <rltk.hpp>
#include <vector>
#include "../planet/region.hpp"

using namespace rltk;

struct designations_t {

	std::vector<uint8_t> mining;

	designations_t() {
		mining.resize(REGION_WIDTH * REGION_HEIGHT * REGION_DEPTH);
		std::fill(mining.begin(), mining.end(), 0);
	}

	std::size_t serialization_identity = 13;

	void save(std::ostream &lbfile) {
		std::size_t size = mining.size();
		serialize(lbfile, size);
		for (uint8_t &n : mining) {
			serialize(lbfile, n);
		}
	}

	static designations_t load(std::istream &lbfile) {
		designations_t c;
		std::size_t size;
		deserialize(lbfile, size);
		for (std::size_t i=0; i<size; ++i) {
			deserialize(lbfile, c.mining[i]);
		}
		return c;
	}
};