#pragma once

#include <cereal/cereal.hpp>
#include "position.hpp"
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"
#include "../bengine/path_finding.hpp"

enum sentient_goal_t { SENTIENT_GOAL_IDLE, SENTIENT_GOAL_FLEE, SENTIENT_GOAL_CHARGE, SENTIENT_GOAL_KILL };

struct sentient_ai {
	sentient_ai() {}
	sentient_ai(const std::size_t cid, const int aggro) : aggression(aggro), civ_id(cid) {}
	int aggression = 0;
    std::size_t civ_id = 0;
	bool hostile = false;
	sentient_goal_t goal = SENTIENT_GOAL_IDLE;
	std::size_t target;
	int days_since_arrival = 0;

	// Not serialized
	std::shared_ptr<bengine::navigation_path<position_t>> current_path;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( aggression, civ_id, hostile, goal, target, days_since_arrival ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<sentient_ai>>)