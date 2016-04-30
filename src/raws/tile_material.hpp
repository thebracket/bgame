#pragma once

#include <rltk.hpp>

namespace raw {
struct tile_material {
	std::size_t index;
	std::string name;
	uint8_t glyph;
	rltk::color_t foreground;
	rltk::color_t background;
};

}