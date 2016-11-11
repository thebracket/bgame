#pragma once

#include <rltk.hpp>

using namespace rltk;

struct name_t {
	std::string first_name;
	std::string last_name;
	std::string xml_identity = "name_t";

	name_t() {}
	name_t(const std::string fn, const std::string ln) : first_name(fn), last_name(ln) {}

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};