#pragma once

#include "../../bengine/shaders.hpp"

struct cursor_shader_t : public bengine::base_shader_t {
	cursor_shader_t();

	unsigned int projection_matrix;
	unsigned int view_matrix;
	unsigned int texture_array;
};