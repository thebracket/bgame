#pragma once

#include <rltk.hpp>

using namespace rltk;

enum sentient_goal_t { SENTIENT_GOAL_IDLE, SENTIENT_GOAL_FLEE, SENTIENT_GOAL_CHARGE, SENTIENT_GOAL_KILL };

struct sentient_ai {
	sentient_ai() {}
	sentient_ai(const int init_mod, const std::size_t pid, const int aggro) : initiative_modifier(init_mod), person_id(pid), aggression(aggro) {}
	int initiative = 0;
	int initiative_modifier = 0;
	int aggression = 0;
    std::size_t person_id;
	bool hostile = false;
	sentient_goal_t goal = SENTIENT_GOAL_IDLE;
	std::size_t target;
	int days_since_arrival = 0;

	// Not serialized
	std::shared_ptr<rltk::navigation_path<position_t>> current_path;

	std::size_t serialization_identity = 25;

	void save(std::ostream &lbfile) {
		serialize(lbfile, initiative);
		serialize(lbfile, initiative_modifier);
        serialize(lbfile, person_id);
		serialize(lbfile, aggression);
		serialize(lbfile, hostile);
		serialize(lbfile, goal);
		serialize(lbfile, target);
		serialize(lbfile, days_since_arrival);
	}

	static sentient_ai load(std::istream &lbfile) {
		sentient_ai c;
		deserialize(lbfile, c.initiative);
		deserialize(lbfile, c.initiative_modifier);
        deserialize(lbfile, c.person_id);
		deserialize(lbfile, c.aggression);
		deserialize(lbfile, c.hostile);
		deserialize(lbfile, c.goal);
		deserialize(lbfile, c.target);
		deserialize(lbfile, c.days_since_arrival);
		return c;
	}
};