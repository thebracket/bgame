#pragma once

#include <vector>
#include <map>
#include "../position.hpp"

namespace farm_steps {
	constexpr uint8_t CLEAR = 0;
	constexpr uint8_t FIX_SOIL = 1;
	constexpr uint8_t PLANT_SEEDS = 2;
	constexpr uint8_t GROWING = 3;
}

struct farm_cycle_t {
	farm_cycle_t() = default;
	farm_cycle_t(const uint8_t st, const std::string t, const size_t seed) noexcept : state(st), seed_type(t), seed_id(seed) {}
	uint8_t state = 0;
	std::string seed_type = "";
	int days_since_weeded = 0;
	int days_since_watered = 0;
	bool fertilized = false;
	std::size_t seed_id = 0;	
};

struct farming_designations_t {

	std::vector<std::pair<bool, position_t>> harvest;
	std::map<int, farm_cycle_t> farms;
};

