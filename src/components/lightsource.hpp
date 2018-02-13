#pragma once

#include <vector>
#include "../bengine/color_t.hpp"

struct lightsource_t {
	lightsource_t() = default;
	lightsource_t(const int r, const bengine::color_t col) noexcept : radius(r), color(col) {}
	lightsource_t(const int r, const bengine::color_t col, const bool alert) noexcept : radius(r), color(col), alert_status(alert) {}

	int radius = 0;
    bengine::color_t color{1.0f, 1.0f, 1.0f};
	bool alert_status = false;

	// Non-persistent
	std::vector<int> lit_cache;
};
