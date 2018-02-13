#pragma once

#include <unordered_map>
#include "../bengine/color_t.hpp"
#include "position.hpp"
#include "helpers/unbuild_t.hpp"

struct designations_t {

	std::unordered_map<std::size_t, position_t> chopping;
	std::vector<std::pair<bool, position_t>> guard_points;
	std::vector<unbuild_t> deconstructions;
	std::vector<std::size_t> levers_to_pull;
	int current_power = 10;
	uint64_t current_cash = 100;

	uint8_t standing_order_idle_move = 1;
	uint8_t standing_order_wildlife_treatment = 1;
	uint8_t standing_order_upgrade = 1;

	// Not serialized
	bengine::color_t alert_color{1.0f, 1.0f, 1.0f};
	int total_capacity = 10;	
};

