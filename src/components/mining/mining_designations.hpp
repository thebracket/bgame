#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs_impl.hpp"
#include <cereal/types/array.hpp>

enum mining_type_t { MINE_DIG, MINE_CHANNEL, MINE_RAMP, MINE_STAIRS_UP, MINE_STAIRS_DOWN, MINE_STAIRS_UPDOWN, MINE_DELETE };

struct mining_designations_t {

	mining_designations_t() {}

	std::map<int, uint8_t> mining_targets;
	int brush_type = 0;
	int brush_size_x = 1;
	int brush_size_y = 1;
	int mine_mode = 0;
	int radius = 1;
	bool stairs_helper = true;
	int stairs_depth = 1;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(mining_targets, brush_type, brush_size_x, brush_size_y, mine_mode, radius, stairs_helper, stairs_depth);
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<mining_designations_t>>)
