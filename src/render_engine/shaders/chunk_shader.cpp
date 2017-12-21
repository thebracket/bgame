#include "stdafx.h"
#include "chunk_shader.hpp"

chunk_shader_t::chunk_shader_t() {
	load_shader("game_assets/chunkshader_vertex.glsl", "game_assets/chunkshader_fragment.glsl");

	view_matrix = get_uniform("view_matrix");
	projection_matrix = get_uniform("projection_matrix");
	textureArray = get_uniform("textureArray");
}