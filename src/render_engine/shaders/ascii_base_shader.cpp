#include "ascii_base_shader.hpp"

ascii_base_shader_t::ascii_base_shader_t() {
	load_shader("game_assets/shaders45/ascii/ascii_base_vertex.glsl", "game_assets/shaders45/ascii/ascii_base_fragment.glsl");

	view_matrix = get_uniform("view_matrix");
	projection_matrix = get_uniform("projection_matrix");
	ascii_tex = get_uniform("ascii_tex");
}
