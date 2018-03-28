#pragma once

#include <vector>
#include <tuple>

struct receives_signal_t {
    bool active = false;
	std::vector<std::tuple<int,int,int,int,int>> receives_from;
};
