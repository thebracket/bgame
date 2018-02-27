#include "worldgen_shader.hpp"

worldgen_shader_t::worldgen_shader_t() {
	load_shader("game_assets/shaders45/worldgen/worldgenshader_vertex.glsl", "game_assets/shaders45/worldgen/worldgenshader_fragment.glsl");

	view_matrix = get_uniform("view");
	projection_matrix = get_uniform("projection_mat");
	model_matrix = get_uniform("model");
}
