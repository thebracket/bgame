#pragma once

#include <rltk.hpp>

using namespace rltk;

struct wildlife_group {
	wildlife_group() {}
    uint8_t group_id;

	std::size_t serialization_identity = 21;

	void save(std::ostream &lbfile) {
        serialize(lbfile, group_id);
	}

	static wildlife_group load(std::istream &lbfile) {
		wildlife_group c;
        deserialize(lbfile, c.group_id);
		return c;
	}
};