#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs_impl.hpp"
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include "../position.hpp"

struct farming_designations_t {

	farming_designations_t() {}

	std::vector<std::pair<bool, position_t>> harvest;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(harvest);
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<farming_designations_t>>)
