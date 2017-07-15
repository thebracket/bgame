#pragma once

#include "base_shader.hpp"

namespace gl {
    struct render_shader_t : public base_shader_t {
        render_shader_t();

        GLint albedo_tex_loc;
        GLint position_tex_loc;
        GLint normal_tex_loc;
        GLint light_pos_tex_loc;
        GLint light_col_tex_loc;
        GLint flag_tex_loc;
        GLint camera_position_loc;
        GLint specular_tex_loc;
    };
}