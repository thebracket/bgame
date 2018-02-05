#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs_impl.hpp"
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include "../position.hpp"

namespace farm_steps {
	constexpr uint8_t CLEAR = 0;
	constexpr uint8_t FIX_SOIL = 1;
	constexpr uint8_t PLANT_SEEDS = 2;
	constexpr uint8_t GROWING = 3;
}

struct farm_cycle_t {
	uint8_t state = 0;
	std::string seed_type = "";
	int days_since_weeded = 0;
	int days_since_watered = 0;
	bool fertilized = false;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(state, seed_type, days_since_weeded, days_since_watered, fertilized);
	}
};

struct farming_designations_t {

	farming_designations_t() {}

	std::vector<std::pair<bool, position_t>> harvest;
	std::map<int, farm_cycle_t> farms;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(harvest, farms);
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<farming_designations_t>>)
