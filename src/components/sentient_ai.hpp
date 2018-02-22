#pragma once

#include "position.hpp"
#include <memory>
#include "../systems/helpers/pathfinding.hpp"

enum sentient_goal_t { SENTIENT_GOAL_IDLE, SENTIENT_GOAL_FLEE, SENTIENT_GOAL_CHARGE, SENTIENT_GOAL_KILL };

struct sentient_ai {
	sentient_ai() = default;
	explicit sentient_ai(const std::size_t cid, const int aggro) noexcept : aggression(aggro), civ_id(cid) {}
	int aggression = 0;
    std::size_t civ_id = 0;
	bool hostile = false;
	sentient_goal_t goal = SENTIENT_GOAL_IDLE;
	std::size_t target = 0;
	int days_since_arrival = 0;

	// Not serialized
	std::shared_ptr<navigation_path_t> current_path;
	
};
