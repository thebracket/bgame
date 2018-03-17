#pragma once

#include <vector>
#include <unordered_set>

struct viewshed_t {
	viewshed_t() = default;
	viewshed_t(const int radius, const bool pen, const bool good_guy=true) noexcept : viewshed_radius(radius), penetrating(pen), good_guy_visibility(good_guy) {}

	int viewshed_radius = 0;
	bool penetrating = false;
	bool good_guy_visibility = true;
	std::unordered_set<std::size_t> visible_entities;

	// Non-persistent
	std::unordered_set<int> visible_cache;
};
