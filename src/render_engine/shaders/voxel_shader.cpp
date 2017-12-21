#include "stdafx.h"
#include "voxel_shader.hpp"

voxel_shader_t::voxel_shader_t() {
	load_shader("game_assets/voxel_vertex.glsl", "game_assets/voxel_fragment.glsl");

	projection_matrix = get_uniform("projection_matrix");
	view_matrix = get_uniform("view_matrix");
	texSize = get_uniform("texSize");
}