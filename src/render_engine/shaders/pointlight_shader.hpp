#pragma once

#include "../../bengine/shaders.hpp"

struct pointlight_shader_t : public bengine::base_shader_t {
	pointlight_shader_t();

	unsigned int light_pos;
	unsigned int far_plane;
	unsigned int shadow_matrices0;
	unsigned int shadow_matrices1;
	unsigned int shadow_matrices2;
	unsigned int shadow_matrices3;
	unsigned int shadow_matrices4;
	unsigned int shadow_matrices5;
};