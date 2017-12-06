#include "lightstage_shader.hpp"

lightstage_shader_t::lightstage_shader_t() {
	load_shader("game_assets/lightpass_vertex.glsl", "game_assets/lightpass_fragment.glsl");

	albedo_tex = get_uniform("albedo_tex");
	normal_tex = get_uniform("normal_tex");
	position_tex = get_uniform("position_tex");
	ao_tex = get_uniform("ao_tex");
	info_tex = get_uniform("info_tex");
	light_pos_tex = get_uniform("light_pos_tex");
	light_col_tex = get_uniform("light_col_tex");
	sun_depth_tex = get_uniform("sun_depth_tex");
	moon_depth_tex = get_uniform("moon_depth_tex");
	noise_tex = get_uniform("noise_tex");
	gbuffer_depth_tex = get_uniform("gbuffer_depth_tex");

	camera_position = get_uniform("camera_position");
	sun_direction = get_uniform("sun_direction");
	sun_color = get_uniform("sun_color");
	moon_direction = get_uniform("moon_direction");
	moon_color = get_uniform("moon_color");
	sun_projection = get_uniform("sun_projection");
	sun_modelview = get_uniform("sun_modelview");
	moon_projection = get_uniform("moon_projection");
	moon_modelview = get_uniform("moon_modelview");
	projection = get_uniform("projection");
}