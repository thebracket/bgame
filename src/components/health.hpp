#pragma once

#include <rltk.hpp>
#include <vector>
#include "helpers/health_part_t.hpp"

using namespace rltk;

struct health_t {
	int max_hitpoints;
	int current_hitpoints;
	bool unconscious = false;
	bool blind = false;
	bool immobile = false;
	bool slow = false;
	bool no_grasp = false;
	std::vector<health_part_t> parts;

	health_t() {}

	std::string xml_identity = "health_t";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};