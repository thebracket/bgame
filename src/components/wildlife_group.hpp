#pragma once

#include <rltk.hpp>

using namespace rltk;

struct wildlife_group {
	wildlife_group() {}
	wildlife_group(uint8_t &id) : group_id(id) {}
    uint8_t group_id;

	std::string xml_identity = "wildlife_group";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};