#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include "position.hpp"
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

enum sentient_goal_t { SENTIENT_GOAL_IDLE, SENTIENT_GOAL_FLEE, SENTIENT_GOAL_CHARGE, SENTIENT_GOAL_KILL };

struct sentient_ai {
	sentient_ai() {}
	sentient_ai(const std::size_t pid, const int aggro) : aggression(aggro), person_id(pid) {}
	int aggression = 0;
    std::size_t person_id;
	bool hostile = false;
	sentient_goal_t goal = SENTIENT_GOAL_IDLE;
	std::size_t target;
	int days_since_arrival = 0;

	// Not serialized
	std::shared_ptr<rltk::navigation_path<position_t>> current_path;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( aggression, person_id, hostile, goal, target, days_since_arrival ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<sentient_ai>>)