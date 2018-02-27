#include "chunk_shader.hpp"

chunk_shader_t::chunk_shader_t() {
	load_shader("game_assets/shaders45/chunkshader_vertex.glsl", "game_assets/shaders45/chunkshader_fragment.glsl");

	camera_block_index = get_block_index("camera_data");
	textureArray = get_uniform("textureArray");
}