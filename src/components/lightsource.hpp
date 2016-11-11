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

	std::string xml_identity = "lightsource_t";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};