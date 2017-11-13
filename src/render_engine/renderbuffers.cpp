#include "renderbuffers.hpp"

namespace render {
	std::unique_ptr<gbuffer_t> gbuffer;
	std::unique_ptr<base_lit_buffer_t> light_stage_buffer;
	std::unique_ptr<hdr_buffer_t> hdr_buffer;
	std::unique_ptr<bloom_pingpong_t> bloom_buffer;

	void build_framebuffers(const int &screen_w, const int &screen_h) {
		gbuffer = std::make_unique<gbuffer_t>(screen_w, screen_h);
		if (!light_stage_buffer) light_stage_buffer = std::make_unique<base_lit_buffer_t>(screen_w, screen_h);
		if (!hdr_buffer) hdr_buffer = std::make_unique<hdr_buffer_t>(screen_w, screen_h);
		if (!bloom_buffer) bloom_buffer = std::make_unique<bloom_pingpong_t>(screen_w, screen_h);
	}
}
