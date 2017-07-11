#include "final_shader.hpp"

namespace gl {
    final_shader_t::final_shader_t() {
        load("world_defs/shaders/final_vertex.glsl",
             "world_defs/shaders/final_fragment.glsl");

        intermediate_tex_loc = get_uniform_location("intermediate_tex");
        exposure_loc = get_uniform_location("exposure");
    }
}