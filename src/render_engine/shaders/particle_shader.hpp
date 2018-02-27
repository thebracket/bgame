#pragma once

#include "../../bengine/shaders.hpp"

struct particle_shader_t : public bengine::base_shader_t {
	particle_shader_t();

	unsigned int projection_matrix;
	unsigned int view_matrix;
};