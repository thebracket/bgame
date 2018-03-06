#pragma once

#include "../../bengine/shaders.hpp"

struct pointlight_shader_t : public bengine::base_shader_t {
	pointlight_shader_t();

	unsigned int light_block_index;
	unsigned int light_index;
};