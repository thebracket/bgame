#pragma once

#include <rltk.hpp>

using namespace rltk;

struct corpse_harvestable {
	corpse_harvestable() {}
    corpse_harvestable(const std::string &tag) : creature_tag(tag) {}
    uint32_t ticks_since_death = 0;
    std::string creature_tag = "";
	bool claimed = false;

	std::string xml_identity = "corpse_harvestable";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};