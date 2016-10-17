#pragma once

#include <istream>
#include <ostream>

struct health_part_t {
	std::string part = "";
	int max_hitpoints = 0;
	int current_hitpoints = 0;
	int size = 0;

	void save(std::ostream &f);
	void load(std::istream &f);
};