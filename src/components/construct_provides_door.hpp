#pragma once

#include <rltk.hpp>

using namespace rltk;

struct construct_door_t {
	bool locked = false;

	construct_door_t() {}

	std::string xml_identity = "construct_door_t";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};