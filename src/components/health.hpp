#pragma once

#include <rltk.hpp>
#include <vector>
#include "helpers/health_part_t.hpp"

using namespace rltk;

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

	void save(std::ostream &lbfile);
	static health_t load(std::istream &lbfile);
};