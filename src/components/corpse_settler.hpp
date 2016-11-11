#pragma once

#include <rltk.hpp>

using namespace rltk;

struct corpse_settler {
	corpse_settler() {}
	corpse_settler(const std::string cause) : cause_of_death(cause) {}
    uint32_t ticks_since_death = 0;
	std::string cause_of_death = "";

	std::string xml_identity = "corpse_settler";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};