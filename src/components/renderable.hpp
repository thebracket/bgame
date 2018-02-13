#pragma once

#include "../bengine/color_t.hpp"

struct renderable_t {
	uint16_t glyph = 0;
	uint16_t glyph_ascii = 0;
	int vox = 0;
	bengine::color_t foreground{};
	bengine::color_t background{};

	renderable_t() = default;
	renderable_t(const uint16_t ch, const uint16_t cha, const bengine::color_t fg, const bengine::color_t bg, const int vx=0) noexcept : glyph(ch), glyph_ascii(cha), vox(vx), foreground(fg), background(bg) {}	
};
