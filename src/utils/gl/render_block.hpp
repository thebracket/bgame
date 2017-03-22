#pragma once

#include <rltk.hpp>
#include <rltk.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

struct v3d {
    GLfloat x, y, z;
};
struct t3d {
    GLfloat tx, ty;
};
struct c3d {
    GLfloat r, g, b;
};

struct render_block {
    std::vector<v3d> vertices;
    std::vector<t3d> tvertices;
    std::vector<c3d> cvertices;
    std::vector<v3d> normals;
    std::vector<v3d> screen_index;


    void reset();

    float light_red(const rltk::vchar &c) const noexcept;
    float light_green(const rltk::vchar &c) const noexcept;
    float light_blue(const rltk::vchar &c) const noexcept;

    void add_floor(const int &x, const int &y, const int &z, const rltk::vchar &c);
    void add_decal(const int &x, const int &y, const int &z, const rltk::vchar &c);
    void add_cube(const int &x, const int &y, const int &z, const rltk::vchar &c);
    void add_fractional_height_cube(const int &x, const int &y, const int &z, const rltk::vchar &c, const float &height);

    void render() const noexcept;
    void render_index() const noexcept;
};

void calculate_texture_info();
