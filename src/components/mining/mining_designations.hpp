#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs_impl.hpp"
#include <cereal/types/array.hpp>

enum mining_type_t { MINE_DIG, MINE_CHANNEL, MINE_RAMP, MINE_STAIRS_UP, MINE_STAIRS_DOWN, MINE_STAIRS_UPDOWN };

struct mining_designations_t {

	mining_designations_t() {}

	std::array<std::map<int, mining_type_t>, 8> mining_targets;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(mining_targets);
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<mining_designations_t>>)
