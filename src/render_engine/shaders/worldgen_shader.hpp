#pragma once

#include "../../bengine/shaders.hpp"

struct worldgen_shader_t : public bengine::base_shader_t {
	worldgen_shader_t();

	unsigned int projection_matrix;
	unsigned int view_matrix;
	unsigned int model_matrix;
};