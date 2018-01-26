#include "stdafx.h"
#include "renderbuffers.hpp"

namespace render {
	gbuffer_t * gbuffer;
	gbuffer_t * gbuffer_alternate;
	bool use_gbuffer1 = true;

	std::unique_ptr<gbuffer_t> gbuffer1;
	std::unique_ptr<gbuffer_t> gbuffer2;
	std::unique_ptr<base_lit_buffer_t> light_stage_buffer;
	std::unique_ptr<hdr_buffer_t> hdr_buffer;
	std::unique_ptr<bloom_pingpong_t> bloom_buffer;

	void build_framebuffers(const int &screen_w, const int &screen_h) {
		gbuffer1 = std::make_unique<gbuffer_t>(screen_w, screen_h);
		gbuffer2 = std::make_unique<gbuffer_t>(screen_w, screen_h);
		gbuffer = gbuffer1.get();
		if (!light_stage_buffer) light_stage_buffer = std::make_unique<base_lit_buffer_t>(screen_w, screen_h);
		if (!hdr_buffer) hdr_buffer = std::make_unique<hdr_buffer_t>(screen_w, screen_h);
		if (!bloom_buffer) bloom_buffer = std::make_unique<bloom_pingpong_t>(screen_w, screen_h);
	}

	void swap_buffers() {
		use_gbuffer1 = !use_gbuffer1;
		gbuffer = use_gbuffer1 ? gbuffer1.get() : gbuffer2.get();
		gbuffer_alternate = use_gbuffer1 ? gbuffer2.get() : gbuffer1.get();
	}
}
