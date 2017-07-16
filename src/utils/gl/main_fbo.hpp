#pragma once

#include "gl_include.hpp"

namespace map_render {

    extern bool loaded_fbo;

    extern GLuint mouse_pick_texture;
    extern GLuint mouse_pick_fbo;
    extern GLuint mouse_pick_depth;
    extern GLuint render_texture;
    extern GLuint normal_texture;
    extern GLuint interpolated_pos_texture;
    extern GLuint light_position_texture;
    extern GLuint light_color_texture;
    extern GLuint flag_texture;
    extern GLuint specular_texture;

    extern GLuint intermediate_fbo;
    extern GLuint intermediate_texture;

    void load_fbo();
    void load_intermediate_fbo();
}