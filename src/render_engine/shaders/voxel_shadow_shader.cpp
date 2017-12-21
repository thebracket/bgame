#include "stdafx.h"
#include "voxel_shadow_shader.hpp"

voxel_shadow_shader_t::voxel_shadow_shader_t() {
	load_shader("game_assets/voxel_shadow_vertex.glsl", "game_assets/voxel_shadow_fragment.glsl");

	combined_matrix = get_uniform("combined_matrix");
	texSize = get_uniform("texSize");
	far_plane = get_uniform("far_plane");
	lightPos = get_uniform("lightPos");
}
