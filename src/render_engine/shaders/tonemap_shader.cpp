#include "tonemap_shader.hpp"

tonemap_shader_t::tonemap_shader_t() {
	load_shader("game_assets/shaders45/tonemap_vertex.glsl", "game_assets/shaders45/tonemap_fragment.glsl");

	hdr_tex = get_uniform("hdr_tex");
}
