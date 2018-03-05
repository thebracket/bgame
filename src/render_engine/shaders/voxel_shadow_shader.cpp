#include "voxel_shadow_shader.hpp"

voxel_shadow_shader_t::voxel_shadow_shader_t() {
	load_shader("game_assets/shaders45/voxel_shadow_vertex.glsl", "game_assets/shaders45/voxel_shadow_fragment.glsl", "game_assets/shaders45/voxel_shadow_geometry.glsl");

	far_plane = get_uniform("far_plane");
	light_pos = get_uniform("lightPos");
	shadow_matrices0 = get_uniform("shadowMatrices[0]");
	shadow_matrices1 = get_uniform("shadowMatrices[1]");
	shadow_matrices2 = get_uniform("shadowMatrices[2]");
	shadow_matrices3 = get_uniform("shadowMatrices[3]");
	shadow_matrices4 = get_uniform("shadowMatrices[4]");
	shadow_matrices5 = get_uniform("shadowMatrices[5]");
	instance_block_index = get_resource_index("instance_data");
}
