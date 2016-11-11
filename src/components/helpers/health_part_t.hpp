#pragma once

#include <istream>
#include <ostream>
#include <rltk.hpp>

struct health_part_t {
	std::string part = "";
	int max_hitpoints = 0;
	int current_hitpoints = 0;
	int size = 0;

	void to_xml(rltk::xml_node * c);
	void from_xml(rltk::xml_node * c);
};