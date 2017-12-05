#pragma once

#include "../../bengine/shaders.hpp"

struct lightstage_shader_t : public bengine::base_shader_t {
	lightstage_shader_t();

	unsigned int albedo_tex;
	unsigned int normal_tex;
	unsigned int position_tex;
	unsigned int ao_tex;
	unsigned int info_tex;
	unsigned int light_pos_tex;
	unsigned int light_col_tex;
	unsigned int sun_depth_tex;
	unsigned int moon_depth_tex;

	unsigned int camera_position;
	unsigned int sun_direction;
	unsigned int sun_color;
	unsigned int moon_direction;
	unsigned int moon_color;
	unsigned int sun_projection;
	unsigned int sun_modelview;
	unsigned int moon_projection;
	unsigned int moon_modelview;

};
