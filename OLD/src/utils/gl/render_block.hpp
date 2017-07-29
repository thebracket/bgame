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
struct v4d {
    GLfloat x, y, z, w;
};

struct render_block {
    std::vector<v3d> vertices;
    std::vector<t3d> tvertices;
    std::vector<c3d> cvertices;
    std::vector<v3d> normals;
    std::vector<v4d> screen_index;

    void reset();

    void add_floor(const int &x, const int &y, const int &z, const rltk::vchar &c, const GLfloat &billboard_mode=0);
    void add_standup(const int &x, const int &y, const int &z, const rltk::vchar &c, const GLfloat &billboard_mode=1.0f);
    void add_decal(const int &x, const int &y, const int &z, const rltk::vchar &c, const GLfloat &billboard_mode=0);
    void add_cube(const int &x, const int &y, const int &z, const rltk::vchar &c, const GLfloat &billboard_mode=0);
    void add_fractional_height_cube(const int &x, const int &y, const int &z, const rltk::vchar &c, const float &height, const GLfloat &billboard_mode=0);

    void render(const GLuint &program_id) const noexcept;

private:
    std::tuple<float,float,float> altitude_color_bias(const rltk::vchar &c, const int &z) const noexcept;
    std::tuple<float, float, float, float, float, float, float, float> size_calcs(const rltk::vchar &c)
            const noexcept;
    std::tuple<float, float, float> screenspace_calc(const int &x, const int &y, const int &z) const noexcept;

    float light_red(const rltk::vchar &c) const noexcept;
    float light_green(const rltk::vchar &c) const noexcept;
    float light_blue(const rltk::vchar &c) const noexcept;

    void primitives_floor(const float &light_r, const float &light_g, const float &light_b, const float &tex_xf,
                          const float &tex_yf, const float &SX, const float &SY, const float &SZ,
                          const float &billboard_mode);
    void primitives_cube_left(const float &light_r, const float &light_g, const float &light_b, const float &tex_xf,
                              const float &tex_yf, const float &SX, const float &SY, const float &SZ, const float &billboard_mode,
                              const float &X, const float &Y, const float &Z, const float &vsize, const float height=1.0f);
    void primitives_cube_right(const float &light_r, const float &light_g, const float &light_b, const float &tex_xf,
                              const float &tex_yf, const float &SX, const float &SY, const float &SZ, const float &billboard_mode,
                              const float &X, const float &Y, const float &Z, const float &vsize, const float height=1.0f);
    void primitives_cube_north(const float &light_r, const float &light_g, const float &light_b, const float &tex_xf,
                               const float &tex_yf, const float &SX, const float &SY, const float &SZ, const float &billboard_mode,
                               const float &X, const float &Y, const float &Z, const float &vsize, const float height=1.0f);
    void primitives_cube_south(const float &light_r, const float &light_g, const float &light_b, const float &tex_xf,
                               const float &tex_yf, const float &SX, const float &SY, const float &SZ, const float &billboard_mode,
                               const float &X, const float &Y, const float &Z, const float &vsize, const float height=1.0f);
    void primitives_cube_ceiling(const float &light_r, const float &light_g, const float &light_b, const float &tex_xf,
                               const float &tex_yf, const float &SX, const float &SY, const float &SZ, const float &billboard_mode,
                               const float &X, const float &Y, const float &Z, const float &vsize, const float height=1.0f);
};

void calculate_texture_info();
