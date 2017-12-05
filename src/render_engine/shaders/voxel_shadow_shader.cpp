#include "voxel_shadow_shader.hpp"

voxel_shadow_shader_t::voxel_shadow_shader_t() {
	load_shader("game_assets/voxel_shadow_vertex.glsl", "game_assets/voxel_shadow_fragment.glsl");

	projection_matrix = get_uniform("projection_matrix");
	view_matrix = get_uniform("view_matrix");
	texSize = get_uniform("texSize");
}
