#include "chunk_depth_shader.hpp"

chunk_depth_shader_t::chunk_depth_shader_t() {
	load_shader("game_assets/chunkshader_depth_vertex.glsl", "game_assets/chunkshader_depth_fragment.glsl");

	view_matrix = get_uniform("view_matrix");
	projection_matrix = get_uniform("projection_matrix");
	camera_position = get_uniform("camera_position");
	normal_matrix = get_uniform("normalMatrix");
}