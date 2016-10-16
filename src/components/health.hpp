#pragma once

#include <rltk.hpp>
#include <vector>
#include "../utils/serialization_utils.hpp"

using namespace rltk;

struct health_part_t {
	std::string part;
	int max_hitpoints;
	int current_hitpoints;
	int size;

	void save(std::ostream &f) {
		Serialize(f, part, max_hitpoints, current_hitpoints, size);
	}

	void load(std::istream &f) {
		Deserialize(f, part, max_hitpoints, current_hitpoints, size);
	}
};

struct health_t {
	int max_hitpoints;
	int current_hitpoints;
	bool unconscious = false;
	bool blind = false;
	bool immobile = false;
	bool slow = false;
	bool no_grasp = false;
	std::vector<health_part_t> parts;

	health_t() {}

	std::size_t serialization_identity = 7;

	void save(std::ostream &lbfile) {
		Serialize(lbfile, max_hitpoints, current_hitpoints, unconscious, blind, immobile, slow, no_grasp, parts);
	}

	static health_t load(std::istream &lbfile) {
		health_t c;
		Deserialize(lbfile, c.max_hitpoints, c.current_hitpoints, c.unconscious, c.blind, c.immobile, c.slow, c.no_grasp, c.parts);
		return c;
	}
};