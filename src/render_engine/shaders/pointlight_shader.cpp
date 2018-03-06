#include "pointlight_shader.hpp"

pointlight_shader_t::pointlight_shader_t() {
	load_shader("game_assets/shaders45/pointlight_vertex.glsl", "game_assets/shaders45/pointlight_fragment.glsl", "game_assets/shaders45/pointlight_geometry.glsl");

	light_block_index = get_block_index("light_data");
	light_index = get_uniform("light_index");
}
