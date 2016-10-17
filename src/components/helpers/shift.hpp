#pragma once

#include <array>
#include <istream>
#include <ostream>

enum shift_type_t { SLEEP_SHIFT, LEISURE_SHIFT, WORK_SHIFT };

struct shift_t {
	std::string shift_name;
	std::array<shift_type_t, 24> hours;
    void save(std::ostream &f);
    void load(std::istream &f);
};
