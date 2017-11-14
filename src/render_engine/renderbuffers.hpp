#pragma once

#include "../stdafx.h"
#include "fbo/base_lit_buffer.hpp"
#include "fbo/gbuffer.hpp"
#include "fbo/bloom_ping_pong.hpp"
#include "fbo/hdr_buffer.hpp"
#include "fbo/directional_light_buffer.hpp"

namespace render {
	extern std::unique_ptr<gbuffer_t> gbuffer;
	extern std::unique_ptr<base_lit_buffer_t> light_stage_buffer;
	extern std::unique_ptr<hdr_buffer_t> hdr_buffer;
	extern std::unique_ptr<bloom_pingpong_t> bloom_buffer;
	extern std::unique_ptr<directional_light_buffer_t> sun_buffer;
	extern std::unique_ptr<directional_light_buffer_t> moon_buffer;

	void build_framebuffers(const int &screen_w, const int &screen_h);
}
