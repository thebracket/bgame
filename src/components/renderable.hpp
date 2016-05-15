#pragma once

#include <rltk.hpp>

using namespace rltk;

struct renderable_t {
	uint8_t glyph;
	color_t foreground;
	color_t background;
	std::size_t serialization_identity = 4;

	renderable_t() {}
	renderable_t(const uint8_t ch, const color_t fg, const color_t bg) : glyph(ch), foreground(fg), background(bg) {}

	void save(std::ostream &lbfile) {
		serialize(lbfile, glyph);
		serialize(lbfile, foreground);
		serialize(lbfile, background);
	}

	static renderable_t load(std::istream &lbfile) {
		renderable_t c;
		deserialize(lbfile, c.glyph);
		deserialize(lbfile, c.foreground);
		deserialize(lbfile, c.background);
		return c;
	}
};