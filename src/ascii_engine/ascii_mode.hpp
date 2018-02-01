#pragma once

namespace render {
	void ascii_render(const double &duration_ms);
	namespace ascii {
		struct glyph_t {
			uint8_t glyph;
			float r, g, b;
			float br, bg, bb;
		};

		extern unsigned int ascii_fbo;
		extern unsigned int ascii_position_tex;
	}
}