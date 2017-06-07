#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

namespace map_render {

    extern bool loaded_fbo;

    extern GLuint mouse_pick_texture;
    extern GLuint mouse_pick_fbo;
    extern GLuint mouse_pick_depth;
    extern GLuint render_texture;
    extern GLuint normal_texture;
    extern GLuint lit_texture;

    void load_fbo();
}