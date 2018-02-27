#pragma once

#include "../../bengine/shaders.hpp"

struct ascii_light_shader_t : public bengine::base_shader_t {
	ascii_light_shader_t();

	unsigned int projection_matrix;
	unsigned int view_matrix;
	unsigned int ascii_tex;
	unsigned int light_position;
	unsigned int light_color;
	unsigned int far_plane;
	unsigned int depth_map;
};