#pragma once

#include <rltk.hpp>
#include <vector>
#include <unordered_set>

using namespace rltk;

struct viewshed_t {
	viewshed_t() {}
	viewshed_t(const int radius, const bool pen, const bool good_guy=true) : viewshed_radius(radius), penetrating(pen), good_guy_visibility(good_guy) {}

	int viewshed_radius = 0;
	bool penetrating = false;
	bool good_guy_visibility = true;
	std::unordered_set<std::size_t> visible_entities;

	// Non-persistent
	std::vector<int> visible_cache;

	std::string xml_identity = "viewshed_t";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};