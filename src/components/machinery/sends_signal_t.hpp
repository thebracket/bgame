#pragma once
#include <vector>

struct sends_signal_t {
	bool active = false;
	std::vector<int> targets;
};
