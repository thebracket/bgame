#include "ascii_light_shader.hpp"

ascii_light_shader_t::ascii_light_shader_t() {
	load_shader("game_assets/shaders45/ascii/ascii_light_vertex.glsl", "game_assets/shaders45/ascii/ascii_light_fragment.glsl");

	view_matrix = get_uniform("view_matrix");
	projection_matrix = get_uniform("projection_matrix");
	ascii_tex = get_uniform("ascii_tex");
	light_position = get_uniform("light_position");
	light_color = get_uniform("light_color");
	far_plane = get_uniform("far_plane");
	depth_map = get_uniform("depthMap");
}
