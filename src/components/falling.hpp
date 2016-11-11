#pragma once

#include <rltk.hpp>

using namespace rltk;

struct falling_t {
	falling_t() {}
    falling_t(const int &d) : distance(d) {}
    int distance = 0;

	std::string xml_identity = "falling_t";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};