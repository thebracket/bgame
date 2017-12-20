#pragma once

#include "../../bengine/shaders.hpp"

struct voxel_shadow_shader_t : public bengine::base_shader_t {
	voxel_shadow_shader_t();

	unsigned int combined_matrix;
	unsigned int texSize;
	unsigned int far_plane;
	unsigned int lightPos;
};