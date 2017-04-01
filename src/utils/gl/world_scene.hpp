#pragma once

#include <rltk.hpp>

namespace world_scene
{
    void reset_all();

    void add_world_floor(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
    void add_world_cube(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
    void add_world_fractional_cube(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx, const float &height);
    void render_world(const GLuint &deferred_id);

    void add_vegetation(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
    void add_decal(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
    void add_simple_renderable(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
    void add_composite_renderable(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
    void add_index_floor(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
    void add_ascii_tile(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx);
}
