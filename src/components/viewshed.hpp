#pragma once

#include <vector>
#include <unordered_set>
#include <cereal/cereal.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"

struct viewshed_t {
	viewshed_t() {}
	viewshed_t(const int radius, const bool pen, const bool good_guy=true) : viewshed_radius(radius), penetrating(pen), good_guy_visibility(good_guy) {}

	int viewshed_radius = 0;
	bool penetrating = false;
	bool good_guy_visibility = true;
	std::unordered_set<std::size_t> visible_entities;

	// Non-persistent
	std::vector<int> visible_cache;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( viewshed_radius, penetrating, good_guy_visibility, visible_entities ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<viewshed_t>>)