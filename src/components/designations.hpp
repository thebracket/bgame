#pragma once

#include <rltk.hpp>
#include <boost/container/flat_map.hpp>
#include "../planet/region.hpp"
#include "position.hpp"

using namespace rltk;

struct designations_t {

	boost::container::flat_map<int, uint8_t> mining;
	boost::container::flat_map<int, position_t> chopping;

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

		size = chopping.size();
		serialize(lbfile, size);

		for (auto it = chopping.begin(); it != chopping.end(); ++it) {
			serialize(lbfile, it->first);
			serialize(lbfile, it->second.x);
			serialize(lbfile, it->second.y);
			serialize(lbfile, it->second.z);
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

		deserialize(lbfile, size);
		for (std::size_t i=0; i<size; ++i) {
			int tree_id;
			int x,y,z;
			deserialize(lbfile, tree_id);
			deserialize(lbfile, x);
			deserialize(lbfile, y);
			deserialize(lbfile, z);
			c.chopping[tree_id] = position_t{x,y,z};
		}

		return c;
	}
};