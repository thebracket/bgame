#pragma once

#include <istream>
#include <ostream>
#include <rltk.hpp>

struct skill_t {
	skill_t() {}
	skill_t(const int8_t lvl, const uint16_t &xp) : skill_level(lvl), experience_gained(xp) {}
	int16_t skill_level = 0;
	uint16_t experience_gained = 0;

	void to_xml(rltk::xml_node * c);
	void from_xml(rltk::xml_node * c);
};