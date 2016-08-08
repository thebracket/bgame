#pragma once

#include <rltk.hpp>
#include <vector>

using namespace rltk;

struct viewshed_t {
	viewshed_t() {}
	viewshed_t(const int radius, const bool pen, const bool good_guy=true) : viewshed_radius(radius), penetrating(pen), good_guy_visibility(good_guy) {}

	int viewshed_radius = 0;
	bool penetrating = false;
	bool good_guy_visibility = true;

	// Non-persistent
	std::vector<int> visible_cache;

	std::size_t serialization_identity = 17;

	void save(std::ostream &lbfile) {
		serialize(lbfile, viewshed_radius);
		serialize(lbfile, penetrating);
		serialize(lbfile, good_guy_visibility);
	}

	static viewshed_t load(std::istream &lbfile) {
		viewshed_t c;
		deserialize(lbfile, c.viewshed_radius);
		deserialize(lbfile, c.penetrating);
		deserialize(lbfile, c.good_guy_visibility);
		return c;
	}
};