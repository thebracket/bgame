#include "voxel_shadow_shader.hpp"

voxel_shadow_shader_t::voxel_shadow_shader_t() {
	load_shader("game_assets/shaders45/voxel_shadow_vertex.glsl", "game_assets/shaders45/voxel_shadow_fragment.glsl");

	combined_matrix = get_uniform("combined_matrix");
	texSize = get_uniform("texSize");
	far_plane = get_uniform("far_plane");
	lightPos = get_uniform("lightPos");
}
