#include "cursor_shader.hpp"

cursor_shader_t::cursor_shader_t() {
	load_shader("game_assets/shaders45/cursor_vertex.glsl", "game_assets/shaders45/cursor_fragment.glsl");

	view_matrix = get_uniform("view_matrix");
	projection_matrix = get_uniform("projection_matrix");
	texture_array = get_uniform("textureArray");
}
