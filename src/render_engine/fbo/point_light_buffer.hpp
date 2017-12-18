#pragma once

namespace render {
	struct point_light_buffer_t {
		point_light_buffer_t(const int &w, const int &h);

		int width, height;
		unsigned int depth_cubemap;
		unsigned int fbo_id;
	};
}