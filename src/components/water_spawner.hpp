#pragma once

#include <rltk.hpp>

using namespace rltk;

struct water_spawner_t {
	water_spawner_t() {}
	water_spawner_t(const uint8_t &t) : spawner_type(t) {}

    uint8_t spawner_type = 0;

	std::string xml_identity = "water_spawner_t";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};