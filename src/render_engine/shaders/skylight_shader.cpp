#include "skylight_shader.hpp"

skylight_shader_t::skylight_shader_t() {
	load_shader("game_assets/shaders45/skylight_vertex.glsl", "game_assets/shaders45/skylight_fragment.glsl");

	light_position = get_uniform("light_position");
	world_position = get_uniform("world_position");
	albedo_tex = get_uniform("albedo_tex");
	normal_tex = get_uniform("normal_tex");
	ao_tex = get_uniform("ao_tex");
	camera_position = get_uniform("camera_position");
	light_color = get_uniform("light_color");
	terrain_flags_buffer_position = get_resource_index("terrain_flags");
}