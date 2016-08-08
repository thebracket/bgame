#pragma once

#include <rltk.hpp>
#include <vector>
#include "boost/container/flat_set.hpp"

using namespace rltk;

struct viewshed_t {
	viewshed_t() {}
	viewshed_t(const int radius, const bool pen, const bool good_guy=true) : viewshed_radius(radius), penetrating(pen), good_guy_visibility(good_guy) {}

	int viewshed_radius = 0;
	bool penetrating = false;
	bool good_guy_visibility = true;
	boost::container::flat_set<std::size_t> visible_entities;

	// Non-persistent
	std::vector<int> visible_cache;

	std::size_t serialization_identity = 17;

	void save(std::ostream &lbfile) {
		serialize(lbfile, viewshed_radius);
		serialize(lbfile, penetrating);
		serialize(lbfile, good_guy_visibility);
		serialize(lbfile, visible_entities.size());
		for (auto it=visible_entities.begin(); it!=visible_entities.end(); ++it) {
			serialize(lbfile, *it);
		}
	}

	static viewshed_t load(std::istream &lbfile) {
		viewshed_t c;
		deserialize(lbfile, c.viewshed_radius);
		deserialize(lbfile, c.penetrating);
		deserialize(lbfile, c.good_guy_visibility);
		std::size_t n_vis;
		deserialize(lbfile, n_vis);
		for (std::size_t i=0; i<n_vis; ++i) {
			std::size_t id;
			deserialize(lbfile, id);
			c.visible_entities.insert(id);
		}
		return c;
	}
};