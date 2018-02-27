#pragma once

namespace render {
	extern bool ssao_setup;
	void setup_ssao();
	void send_samples_to_shader();
}
