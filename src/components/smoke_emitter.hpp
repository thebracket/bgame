#pragma once

#include <rltk.hpp>

using namespace rltk;

struct smoke_emitter_t {
	smoke_emitter_t() {}

	std::string xml_identity = "smoke_emitter_t";

	void to_xml(xml_node * c) {}
	void from_xml(xml_node * c) {}
};