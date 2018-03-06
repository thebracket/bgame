#include "voxel_shadow_shader.hpp"

voxel_shadow_shader_t::voxel_shadow_shader_t() {
	load_shader("game_assets/shaders45/voxel_shadow_vertex.glsl", "game_assets/shaders45/voxel_shadow_fragment.glsl", "game_assets/shaders45/voxel_shadow_geometry.glsl");

	light_block_index = get_block_index("light_data");
	light_index = get_uniform("light_index");
	instance_block_index = get_resource_index("instance_data");
}
