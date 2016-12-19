#pragma once

#include <rltk.hpp>

using namespace rltk;

struct grazer_ai {
	grazer_ai() {}

	std::string xml_identity = "grazer_ai";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};