#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

namespace map_render {
    extern bool loaded_sun_fbo;

    // Shadow-based lighting
    extern GLuint sun_fbo;
    extern GLuint sun_depth_buffer;
    extern GLuint sun_depth_texture;
    extern GLuint sun_render;

    // Light output buffer
    extern GLuint light_fbo;
    extern GLuint light_depth_buffer;
    extern GLuint light_render_pos;
    extern GLuint light_render_col;

    void load_sun_fbo();
}