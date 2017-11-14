#pragma once

namespace render {
	struct directional_light_buffer_t {
		directional_light_buffer_t(const int &w, const int &h);

		int width, height;
		unsigned int fbo_id = 0;

		unsigned int depth_tex = 0;
	};
}
