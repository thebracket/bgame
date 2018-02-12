#pragma once

#include "../position.hpp"
#include "../helpers/building_designation_t.hpp"
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs_impl.hpp"

struct building_designations_t {

	std::vector<building_designation_t> buildings;
	std::vector<std::pair<uint8_t, std::string>> build_orders;

	building_designations_t() = default;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(buildings, build_orders); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<building_designations_t>>)
