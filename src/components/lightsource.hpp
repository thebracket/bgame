#pragma once

#include <rltk.hpp>
#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct lightsource_t {
	lightsource_t() {}
	lightsource_t(const int r, const color_t col) : radius(r), color(col) {}
	lightsource_t(const int r, const color_t col, const bool alert) : radius(r), color(col), alert_status(alert) {}

	int radius = 0;
    color_t color = rltk::colors::WHITE;
	bool alert_status = false;

	// Non-persistent
	std::vector<int> lit_cache;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( radius, color, alert_status ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<lightsource_t>>)