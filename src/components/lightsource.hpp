#pragma once

#include <rltk.hpp>
#include <vector>

using namespace rltk;

struct lightsource_t {
	lightsource_t() {}
	lightsource_t(const int r, const color_t col) : radius(r), color(col) {}
	lightsource_t(const int r, const color_t col, const bool alert) : radius(r), color(col), alert_status(alert) {}

	int radius = 0;
    color_t color;
	bool alert_status = false;

	// Non-persistent
	std::vector<int> lit_cache;

	std::size_t serialization_identity = 19;

	void save(std::ostream &lbfile) {
		serialize(lbfile, radius);
		serialize(lbfile, color);
	}

	static lightsource_t load(std::istream &lbfile) {
		lightsource_t c;
		deserialize(lbfile, c.radius);
		deserialize(lbfile, c.color);
		return c;
	}
};