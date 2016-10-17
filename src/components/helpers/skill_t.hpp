#pragma once

#include <istream>
#include <ostream>

struct skill_t {
	skill_t() {}
	skill_t(const int8_t lvl, const uint16_t &xp) : skill_level(lvl), experience_gained(xp) {}
	int8_t skill_level = 0;
	uint16_t experience_gained = 0;
    void save(std::ostream &f);
    void load(std::istream &f);
};