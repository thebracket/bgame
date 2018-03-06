#pragma once

#include "../../bengine/shaders.hpp"

struct skylight_shader_t : public bengine::base_shader_t {
	skylight_shader_t();

	unsigned int light_position;
	unsigned int world_position;
	unsigned int albedo_tex;
	unsigned int normal_tex;
	unsigned int ao_tex;
	unsigned int camera_position;
	unsigned int light_color;
	unsigned int terrain_flags_buffer_position;
};