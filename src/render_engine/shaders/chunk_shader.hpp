#pragma once

#include "../../bengine/shaders.hpp"

struct chunk_shader_t : public bengine::base_shader_t {
	chunk_shader_t();

	unsigned int camera_block_index;
	unsigned int textureArray;
};