#pragma once

namespace render {
	extern bool ssao_setup;
	extern unsigned int noise_tex;
	void setup_ssao();
	void send_samples_to_shader();
}
