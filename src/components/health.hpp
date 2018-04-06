#pragma once

#include <vector>
#include "helpers/health_part_t.hpp"

struct health_t {
	int max_hitpoints;
	int current_hitpoints;
	bool unconscious = false;
	bool blind = false;
	bool immobile = false;
	bool slow = false;
	bool no_grasp = false;
	int stunned_counter = 0;
	std::vector<health_part_t> parts;	
};
