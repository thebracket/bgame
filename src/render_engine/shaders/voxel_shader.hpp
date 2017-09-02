#pragma once

#include "../../bengine/shaders.hpp"

struct voxel_shader_t : public bengine::base_shader_t {
	voxel_shader_t();

	unsigned int projection_matrix;
	unsigned int view_matrix;
	unsigned int texSize;
};