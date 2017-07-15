#include "render_shader.hpp"

namespace gl {
    render_shader_t::render_shader_t() {
        load("world_defs/shaders/render_vertex.glsl",
             "world_defs/shaders/render_fragment.glsl");

        albedo_tex_loc = get_uniform_location("albedo_tex");
        position_tex_loc = get_uniform_location("position_tex");
        normal_tex_loc = get_uniform_location("normal_tex");
        light_pos_tex_loc = get_uniform_location("light_pos_tex");
        light_col_tex_loc = get_uniform_location("light_col_tex");
        flag_tex_loc = get_uniform_location("flag_tex");
        camera_position_loc = get_uniform_location("cameraPosition");
        specular_tex_loc = get_uniform_location("specular_tex");
    }
}