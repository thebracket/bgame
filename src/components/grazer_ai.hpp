#pragma once

#include <rltk.hpp>

using namespace rltk;

struct grazer_ai {
	grazer_ai() {}
	grazer_ai(const int init_mod) : initiative_modifier(init_mod) {}
	int initiative = 0;
	int initiative_modifier = 0;

	std::string xml_identity = "grazer_ai";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};