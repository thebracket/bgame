#pragma once

#include <rltk.hpp>

using namespace rltk;

struct sentient_ai {
	sentient_ai() {}
	sentient_ai(const int init_mod, const std::size_t pid) : initiative_modifier(init_mod), person_id(pid) {}
	int initiative = 0;
	int initiative_modifier = 0;
	int aggression = 0;
    std::size_t person_id;
	bool hostile = false;

	std::size_t serialization_identity = 25;

	void save(std::ostream &lbfile) {
		serialize(lbfile, initiative);
		serialize(lbfile, initiative_modifier);
        serialize(lbfile, person_id);
		serialize(lbfile, aggression);
		serialize(lbfile, hostile);
	}

	static sentient_ai load(std::istream &lbfile) {
		sentient_ai c;
		deserialize(lbfile, c.initiative);
		deserialize(lbfile, c.initiative_modifier);
        deserialize(lbfile, c.person_id);
		deserialize(lbfile, c.aggression);
		deserialize(lbfile, c.hostile);
		return c;
	}
};