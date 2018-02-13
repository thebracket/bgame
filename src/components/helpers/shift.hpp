#pragma once

#include <array>
#include <string>

enum shift_type_t { SLEEP_SHIFT, LEISURE_SHIFT, WORK_SHIFT };

struct shift_t {
	std::string shift_name;
	std::array<shift_type_t, 24> hours;
};
