#include "voxel_shader.hpp"

voxel_shader_t::voxel_shader_t() {
	load_shader("game_assets/shaders45/voxel_base_vertex.glsl", "game_assets/shaders45/voxel_base_fragment.glsl");

	projection_matrix = get_uniform("projection_matrix");
	view_matrix = get_uniform("view_matrix");
	texSize = get_uniform("texSize");
}