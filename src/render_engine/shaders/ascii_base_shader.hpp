#pragma once

#include "../../bengine/shaders.hpp"

struct ascii_base_shader_t : public bengine::base_shader_t {
	ascii_base_shader_t();

	unsigned int projection_matrix;
	unsigned int view_matrix;
	unsigned int ascii_tex;
};