#pragma once

#include "fbo/base_lit_buffer.hpp"
#include "fbo/gbuffer.hpp"
#include "fbo/bloom_ping_pong.hpp"
#include "fbo/hdr_buffer.hpp"
#include "fbo/point_light_buffer.hpp"
#include <memory>

namespace render {
	extern gbuffer_t * gbuffer;
	extern gbuffer_t * gbuffer_alternate;
	extern std::unique_ptr<base_lit_buffer_t> light_stage_buffer;
	extern std::unique_ptr<hdr_buffer_t> hdr_buffer;
	extern std::unique_ptr<bloom_pingpong_t> bloom_buffer;

	void build_framebuffers(const int &screen_w, const int &screen_h);
	void swap_buffers();
}
