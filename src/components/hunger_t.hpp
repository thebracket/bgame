#pragma once

struct hunger_t {
	int hunger_clock = 24 * 60; // 24 hours in minutes
	bool is_hungry = false;
	bool is_starving = false;
};
