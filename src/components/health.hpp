#pragma once

#include <rltk.hpp>
#include <vector>

using namespace rltk;

struct health_part_t {
	std::string part;
	int max_hitpoints;
	int current_hitpoints;
	int size;
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
		serialize(lbfile, max_hitpoints);
		serialize(lbfile, current_hitpoints);
		serialize(lbfile, unconscious);
		serialize(lbfile, immobile);
		serialize(lbfile, slow);
		serialize(lbfile, no_grasp);
		serialize(lbfile, parts.size());
		for (const health_part_t &part : parts) {
			serialize(lbfile, part.part);
			serialize(lbfile, part.max_hitpoints);
			serialize(lbfile, part.current_hitpoints);
			serialize(lbfile, part.size);
		}
	}

	static health_t load(std::istream &lbfile) {
		health_t c;
		deserialize(lbfile, c.max_hitpoints);
		deserialize(lbfile, c.current_hitpoints);
		deserialize(lbfile, c.unconscious);
		deserialize(lbfile, c.immobile);
		deserialize(lbfile, c.slow);
		deserialize(lbfile, c.no_grasp);
		std::size_t n_parts;
		deserialize(lbfile, n_parts);
		for (std::size_t i=0; i<n_parts; ++i) {
			health_part_t part;
			deserialize(lbfile, part.part);
			deserialize(lbfile, part.max_hitpoints);
			deserialize(lbfile, part.current_hitpoints);
			deserialize(lbfile, part.size);
			c.parts.push_back(part);
		}
		return c;
	}
};