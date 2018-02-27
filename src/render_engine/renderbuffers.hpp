#pragma once

#include "fbo/base_lit_buffer.hpp"
#include "fbo/gbuffer.hpp"
#include "fbo/hdr_buffer.hpp"
#include "fbo/point_light_buffer.hpp"
#include <memory>

namespace render {
	extern gbuffer_t * gbuffer;
	extern std::unique_ptr<base_lit_buffer_t> light_stage_buffer;
	extern std::unique_ptr<hdr_buffer_t> hdr_buffer;

	void build_framebuffers(const int &screen_w, const int &screen_h);
}
