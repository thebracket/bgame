#pragma once

#include "../../bengine/shaders.hpp"

struct voxel_shader_t : public bengine::base_shader_t {
	voxel_shader_t();

	unsigned int camera_block_index;
	unsigned int instance_block_index;
};