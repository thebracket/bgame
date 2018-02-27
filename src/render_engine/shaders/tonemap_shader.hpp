#pragma once

#include "../../bengine/shaders.hpp"

struct tonemap_shader_t : public bengine::base_shader_t {
	tonemap_shader_t();

	unsigned int hdr_tex;
};