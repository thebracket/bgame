#include "lightstage_shader.hpp"

lightstage_shader_t::lightstage_shader_t() {
	load_shader("game_assets/shaders45/lightpass_vertex.glsl", "game_assets/shaders45/lightpass_fragment.glsl");

	albedo_tex = get_uniform("albedo_tex");
	normal_tex = get_uniform("normal_tex");
	position_tex = get_uniform("position_tex");
	ao_tex = get_uniform("ao_tex");
	gbuffer_depth_tex = get_uniform("gbuffer_depth_tex");
	useSSAO = get_uniform("useSSAO");

	camera_position = get_uniform("camera_position");
	moon_color = get_uniform("moon_color");
	projection = get_uniform("projection");

	terrain_flags = get_resource_index("terrain_flags");
}