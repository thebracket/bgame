#include "particle_shader.hpp"

particle_shader_t::particle_shader_t() {
	load_shader("game_assets/shaders45/particle_vertex.glsl", "game_assets/shaders45/particle_fragment.glsl");

	camera_block_index = get_block_index("camera_data");
}
