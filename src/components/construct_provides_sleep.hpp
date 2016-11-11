#pragma once

#include <rltk.hpp>

using namespace rltk;

struct construct_provides_sleep_t {
	bool claimed = false;

	construct_provides_sleep_t() {}

	std::string xml_identity = "construct_provides_sleep_t";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};