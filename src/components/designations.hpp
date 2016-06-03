#pragma once

#include <rltk.hpp>
#include <boost/container/flat_map.hpp>
#include "../planet/region.hpp"

using namespace rltk;

struct designations_t {

	boost::container::flat_map<int, uint8_t> mining;

	designations_t() {
	}

	std::size_t serialization_identity = 13;

	void save(std::ostream &lbfile) {
		std::size_t size = mining.size();
		serialize(lbfile, size);

		for (auto it = mining.begin(); it != mining.end(); ++it) {
			serialize(lbfile, it->first);
			serialize(lbfile, it->second);
		}
	}

	static designations_t load(std::istream &lbfile) {
		designations_t c;
		std::size_t size;
		deserialize(lbfile, size);
		for (std::size_t i=0; i<size; ++i) {
			int idx;
			uint8_t tmp;
			deserialize(lbfile, idx);
			deserialize(lbfile, tmp);
			c.mining[idx] = tmp;
		}
		return c;
	}
};