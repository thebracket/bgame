#include "renderbuffers.hpp"

namespace render {
	gbuffer_t * gbuffer;

	std::unique_ptr<gbuffer_t> gbuffer1;
	std::unique_ptr<base_lit_buffer_t> light_stage_buffer;
	std::unique_ptr<hdr_buffer_t> hdr_buffer;

	void build_framebuffers(const int &screen_w, const int &screen_h) {
		gbuffer1 = std::make_unique<gbuffer_t>(screen_w, screen_h);
		gbuffer = gbuffer1.get();
		if (!light_stage_buffer) light_stage_buffer = std::make_unique<base_lit_buffer_t>(screen_w, screen_h);
		if (!hdr_buffer) hdr_buffer = std::make_unique<hdr_buffer_t>(screen_w, screen_h);
	}
}
