#pragma once

#include <rltk.hpp>
#include <vector>
#include "helpers/health_part_t.hpp"
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct health_t {
	int max_hitpoints;
	int current_hitpoints;
	bool unconscious = false;
	bool blind = false;
	bool immobile = false;
	bool slow = false;
	bool no_grasp = false;
	std::vector<health_part_t> parts;

	health_t() {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( max_hitpoints, current_hitpoints, unconscious, blind, immobile, slow, no_grasp, parts ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<health_t>>)