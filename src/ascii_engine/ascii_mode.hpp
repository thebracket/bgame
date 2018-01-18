#pragma once

namespace render {
	void ascii_render(const double &duration_ms);
	namespace ascii {
		extern unsigned int ascii_fbo;
		extern unsigned int ascii_position_tex;
	}
}