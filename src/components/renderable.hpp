#pragma once

#include <rltk.hpp>

using namespace rltk;

struct renderable_t {
	uint8_t glyph;
	color_t foreground;
	color_t background;
	std::string xml_identity = "renderable_t";

	renderable_t() {}
	renderable_t(const uint8_t ch, const color_t fg, const color_t bg) : glyph(ch), foreground(fg), background(bg) {}

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};