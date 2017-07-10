#pragma once

#include "base_shader.hpp"

namespace gl {
    struct static_model_shader_t : public base_shader_t {
        static_model_shader_t();

        GLint in_position_loc;
        GLint world_position_loc;
        GLint normal_loc;
        GLint texture_position_loc;
        GLint projection_matrix_loc;
        GLint view_matrix_loc;
        GLint camera_position_loc;
        GLint my_color_texture_loc;
        GLint flags_loc;
        GLint light_position_loc;
        GLint light_color_loc;
    };
}