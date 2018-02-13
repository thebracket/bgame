#pragma once

#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"
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

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( radius, color, alert_status ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<lightsource_t>>)