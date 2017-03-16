#include "world_scene.hpp"
#include <vector>
#include <rltk.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include "../../main/game_globals.hpp"

using namespace rltk;

namespace world_scene {

    /* Texture information - stores the size of the texture we are using */
    float texture_w = 384.0F;
    float texture_h = 768.0F;
    float tex_xsize = 24.0f / texture_w;
    float tex_ysize = 24.0f / texture_h;

    void calculate_texture_info() {
        auto texture_size_vec = rltk::get_texture(term(1)->get_font_tag())->getSize();
        texture_w = texture_size_vec.x;
        texture_h = texture_size_vec.y;
        tex_xsize = 24.0f / texture_w;
        tex_ysize = 24.0f / texture_h;
    }

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

        void reset() {
            vertices.clear();
            tvertices.clear();
            cvertices.clear();
        }

        float light_red(const rltk::vchar &c) const noexcept {
            return (float)c.foreground.r / 255.0f;
        }

        float light_green(const rltk::vchar &c) const noexcept {
            return (float)c.foreground.g / 255.0f;
        }

        float light_blue(const rltk::vchar &c) const noexcept {
            return (float)c.foreground.b / 255.0f;
        }

        void add_floor(const int &x, const int &y, const int &z, const rltk::vchar &c)
        {
            const float light_r = light_red(c);
            const float light_g = light_green(c);
            const float light_b = light_blue(c);

            const float X = (float)x;
            const float Y = (float)y;
            const float Z = (float)z;

            const float tex_x = (c.glyph % 16) * 24.0f;
            const float tex_y = (c.glyph / 16) * 24.0f;
            const float tex_xf = (float) tex_x / texture_w;
            const float tex_yf = (float) tex_y / texture_h;

            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});

            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

            vertices.emplace_back(v3d{X,         Z, Y        });
            vertices.emplace_back(v3d{X,         Z, Y + 1.0f});
            vertices.emplace_back(v3d{X + 1.0f, Z, Y + 1.0f});
            vertices.emplace_back(v3d{X + 1.0f, Z, Y        });
        }

        void add_decal(const int &x, const int &y, const int &z, const rltk::vchar &c)
        {
            constexpr float light_r = 1.0f;
            constexpr float light_g = 1.0f;
            constexpr float light_b = 1.0f;

            const float X = (float)x;
            const float Y = (float)y;
            const float Z = (float)z;

            const float tex_x = (c.glyph % 16) * 24.0f;
            const float tex_y = (c.glyph / 16) * 24.0f;
            const float tex_xf = (float) tex_x / texture_w;
            const float tex_yf = (float) tex_y / texture_h;

            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});

            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

            vertices.emplace_back(v3d{X,         Z, Y        });
            vertices.emplace_back(v3d{X,         Z, Y + 1.0f});
            vertices.emplace_back(v3d{X + 1.0f, Z, Y + 1.0f});
            vertices.emplace_back(v3d{X + 1.0f, Z, Y        });
        }

        void add_cube(const int &x, const int &y, const int &z, const rltk::vchar &c) {
            /*
            const float light_r = light_red(c) - (((float)camera_position->region_z - (float)z)*0.15f);
            const float light_g = light_green(c) - (((float)camera_position->region_z - (float)z)*0.15f);
            const float light_b = light_blue(c) - (((float)camera_position->region_z - (float)z)*0.15f);

            const float light_r2 = light_r / 2.0f;
            const float light_g2 = light_g / 2.0f;
            const float light_b2 = light_b / 2.0f;

            const float X = (float)x*10.0f;
            const float Y = (float)y*10.0f;
            const float Z = (float)z*10.0f;

            const float tex_x = (c.glyph % 16) * 24.0f;
            const float tex_y = (c.glyph / 16) * 24.0f;
            const float tex_xf = (float) tex_x / texture_w;
            const float tex_yf = (float) tex_y / texture_h;

            // Front face
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});

            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});

            vertices.emplace_back(v3d{ X+10.0f, Z+10.0f, Y+10.0f });
            vertices.emplace_back(v3d{ X,      Z+10.0f, Y+10.0f });
            vertices.emplace_back(v3d{ X     , Z     , Y+10.0f });
            vertices.emplace_back(v3d{ X+10.0f, Z     , Y+10.0f });

            // Back face
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});

            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});

            vertices.emplace_back(v3d{ X+10.0f, Z     , Y });
            vertices.emplace_back(v3d{ X,      Z     , Y });
            vertices.emplace_back(v3d{ X     , Z+10.0f, Y });
            vertices.emplace_back(v3d{ X+10.0f, Z+10.0f, Y });

            // Left Face
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});

            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});

            vertices.emplace_back(v3d{ X, Z+10.0f, Y+10.0f });
            vertices.emplace_back(v3d{ X, Z+10.0f, Y });
            vertices.emplace_back(v3d{ X, Z, Y });
            vertices.emplace_back(v3d{ X, Z, Y+10.0f });

            // Right Face
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});

            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});

            vertices.emplace_back(v3d{ X+10.0f, Z     , Y+10.0f });
            vertices.emplace_back(v3d{ X+10.0f, Z     , Y });
            vertices.emplace_back(v3d{ X+10.0f, Z+10.0f, Y });
            vertices.emplace_back(v3d{ X+10.0f, Z+10.0f, Y+10.0f });

            // Ceiling
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});
            cvertices.emplace_back(c3d{light_r2, light_g2, light_b2});

            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

            vertices.emplace_back(v3d{X,        Z, Y});
            vertices.emplace_back(v3d{X,        Z, Y + 10.0f});
            vertices.emplace_back(v3d{X + 10.0f, Z, Y + 10.0f});
            vertices.emplace_back(v3d{X + 10.0f, Z, Y});*/
        }

        void render(const GLuint &program_id) const noexcept {
            //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            glColorPointer(3, GL_FLOAT, 0, &cvertices[0]);
            glTexCoordPointer(2, GL_FLOAT, 0, &tvertices[0]);
            glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
            glDrawArrays(GL_QUADS, 0, vertices.size());
        }
    };

    render_block world_geometry;

    /*
     * Reset all buffers. Only used the first time, or when the world geometry changes.
     */
    void reset_all() {
        world_geometry.reset();
    }

    // Adds a world-geometry floor tile
    void add_world_floor(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx) {
        if (current_region->above_ground[idx]) {
            world_geometry.add_floor(x, y, z, c);
        } else {
            world_geometry.add_floor(x, y, z, c);
        }
    }

    // Adds a world-geometry cube (solid)
    void add_world_cube(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx) {
        if (current_region->above_ground[idx]) {
            world_geometry.add_cube(x, y, z, c);
        } else {
            world_geometry.add_cube(x, y, z, c);
        }
    }

    // Renders the world geometry
    void render_world(const GLuint &program_id) {
        glUseProgram(program_id);
        world_geometry.render(program_id);
        glUseProgram(0);
    }

    // Adds renderable vegetation
    void add_vegetation(const int &x, const int &y, const int &z, const rltk::vchar &c) {
        world_geometry.add_decal(x, y, z, c);
    }

    // Adds decals (blood stains)
    void add_decal(const int &x, const int &y, const int &z, const rltk::vchar &c) {
        world_geometry.add_decal(x, y, z, c);
    }

    // Adds simple renderable
    void add_simple_renderable(const int &x, const int &y, const int &z, const rltk::vchar &c) {
        world_geometry.add_decal(x, y, z, c);
    }

    // Adds composite renderable
    void add_composite_renderable(const int &x, const int &y, const int &z, const rltk::vchar &c) {
        world_geometry.add_floor(x, y, z, c);
    }
}
