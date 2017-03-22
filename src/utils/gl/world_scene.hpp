#pragma once

#include <rltk.hpp>

namespace world_scene
{
    void calculate_texture_info();
    void reset_all();
    void build_textures(const GLuint &deferred_id);

    void add_world_floor(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
    void add_world_cube(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
    void render_world(const GLuint &program_id, const GLuint &deferred_id);
    void render_index(const GLuint &program_id);

    void add_vegetation(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
    void add_decal(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
    void add_simple_renderable(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
    void add_composite_renderable(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
}
