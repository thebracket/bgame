#include "lighter_shader.hpp"

lighter_shader_t::lighter_shader_t() {
	load_shader("game_assets/shaders45/lighter_vertex.glsl", "game_assets/shaders45/lighter_fragment.glsl");

	light_position = get_uniform("light_position");
	far_plane = get_uniform("far_plane");
	world_position = get_uniform("world_position");
	depth_map = get_uniform("depthMap");
	albedo_tex = get_uniform("albedo_tex");
	normal_tex = get_uniform("normal_tex");
	ao_tex = get_uniform("ao_tex");
	camera_position = get_uniform("camera_position");
	light_color = get_uniform("light_color");
}