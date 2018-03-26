#pragma once

#include <vector>

struct receives_signal_t {
    bool active = false;
	std::vector<int> receives_from;
};
