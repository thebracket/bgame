#pragma once

struct thirst_t {
	int thirst_clock = 24 * 60; // 24 hours in minutes
	bool is_thirsty = false;
	bool is_dehydrating = false;
};
