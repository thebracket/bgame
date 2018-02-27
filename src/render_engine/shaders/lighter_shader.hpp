#pragma once

#include "../../bengine/shaders.hpp"

struct lighter_shader_t : public bengine::base_shader_t {
	lighter_shader_t();

	unsigned int light_position;
	unsigned int far_plane;
	unsigned int world_position;
	unsigned int depth_map;
	unsigned int albedo_tex;
	unsigned int normal_tex;
	unsigned int ao_tex;
	unsigned int camera_position;
	unsigned int light_color;
};