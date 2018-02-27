#include "particle_shader.hpp"

particle_shader_t::particle_shader_t() {
	load_shader("game_assets/shaders45/particle_vertex.glsl", "game_assets/shaders45/particle_fragment.glsl");

	view_matrix = get_uniform("view_matrix");
	projection_matrix = get_uniform("projection_matrix");
}
