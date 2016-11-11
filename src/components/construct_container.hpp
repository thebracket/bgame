#pragma once

#include <rltk.hpp>

using namespace rltk;

struct construct_container_t {
	construct_container_t() {}

	std::string xml_identity = "construct_container_t";

	void to_xml(xml_node * c) {}
	void from_xml(xml_node * c) {}
};