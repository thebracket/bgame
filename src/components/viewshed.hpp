#pragma once

#include <rltk.hpp>
#include <vector>

using namespace rltk;

struct viewshed_t {
	viewshed_t() {}
	viewshed_t(const int radius, const bool pen) : viewshed_radius(radius), penetrating(pen) {}

	int viewshed_radius = 0;
	bool penetrating = false;

	// Non-persistent
	std::vector<int> visible_cache;

	std::size_t serialization_identity = 17;

	void save(std::ostream &lbfile) {
		serialize(lbfile, viewshed_radius);
		serialize(lbfile, penetrating);
	}

	static viewshed_t load(std::istream &lbfile) {
		viewshed_t c;
		deserialize(lbfile, c.viewshed_radius);
		deserialize(lbfile, c.penetrating);
		return c;
	}
};