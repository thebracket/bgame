#pragma once

#include "../../bengine/shaders.hpp"

struct voxel_shadow_shader_t : public bengine::base_shader_t {
	voxel_shadow_shader_t();

	unsigned int light_block_index;
	unsigned int light_index;
	unsigned int instance_block_index;
};