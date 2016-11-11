#pragma once

#include <rltk.hpp>
#include <string>

using namespace rltk;

struct item_stored_t {
	std::size_t stored_in;
	item_stored_t() {}
	item_stored_t(const std::size_t carrier) : stored_in(carrier) {}

	std::string xml_identity = "item_stored_t";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};