#include "pointlight_shader.hpp"

pointlight_shader_t::pointlight_shader_t() {
	load_shader("game_assets/shaders45/pointlight_vertex.glsl", "game_assets/shaders45/pointlight_fragment.glsl", "game_assets/shaders45/pointlight_geometry.glsl");

	light_pos = get_uniform("lightPos");
	far_plane = get_uniform("far_plane");
	shadow_matrices0 = get_uniform("shadowMatrices[0]");
	shadow_matrices1 = get_uniform("shadowMatrices[1]");
	shadow_matrices2 = get_uniform("shadowMatrices[2]");
	shadow_matrices3 = get_uniform("shadowMatrices[3]");
	shadow_matrices4 = get_uniform("shadowMatrices[4]");
	shadow_matrices5 = get_uniform("shadowMatrices[5]");
}
