#include "voxel_shader.hpp"

voxel_shader_t::voxel_shader_t() {
	load_shader("game_assets/shaders45/voxel_base_vertex.glsl", "game_assets/shaders45/voxel_base_fragment.glsl");

	camera_block_index = get_block_index("camera_data");
	instance_block_index = get_resource_index("instance_data");
}