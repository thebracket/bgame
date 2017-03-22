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
#include "../../systems/render/map_render_system.hpp"
#include "../../systems/render/lighting_system.hpp"

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
        tex_xsize = (24.0f / texture_w);
        tex_ysize = (24.0f / texture_h);
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
        std::vector<v3d> normals;
        std::vector<v3d> screen_index;

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
            const float light_r = light_red(c) - ((camera_position->region_z - (float)z)*0.05f);
            const float light_g = light_green(c) - ((camera_position->region_z - (float)z)*0.05f);
            const float light_b = light_blue(c) - ((camera_position->region_z - (float)z)*0.05f);

            const float X = (float)x;
            const float Y = (float)y;
            const float Z = (float)z;
            const float vsize = 1.0f;

            const float tex_x = (c.glyph % 16) * 24.0f;
            const float tex_y = (c.glyph / 16) * 24.0f;
            const float tex_xf = (float) tex_x / texture_w;
            const float tex_yf = (float) tex_y / texture_h;

            // Floor
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});

            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

            vertices.emplace_back(v3d{X,        Z, Y});
            vertices.emplace_back(v3d{X,        Z, Y + vsize});
            vertices.emplace_back(v3d{X + vsize, Z, Y + vsize});
            vertices.emplace_back(v3d{X + vsize, Z, Y});

            const float SX = X/255.0f;
            const float SY = Y/255.0f;
            const float SZ = Z/255.0f;
            screen_index.emplace_back(v3d{SX,SY,SZ});
            screen_index.emplace_back(v3d{SX,SY,SZ});
            screen_index.emplace_back(v3d{SX,SY,SZ});
            screen_index.emplace_back(v3d{SX,SY,SZ});

            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
        }

        void add_decal(const int &x, const int &y, const int &z, const rltk::vchar &c)
        {
            const float light_r = 1.0f - ((camera_position->region_z - (float)z)*0.05f);
            const float light_g = 1.0f - ((camera_position->region_z - (float)z)*0.05f);
            const float light_b = 1.0f - ((camera_position->region_z - (float)z)*0.05f);

            const float X = (float)x;
            const float Y = (float)y;
            const float Z = (float)z;
            const float vsize = 1.0f;

            const float tex_x = ((c.glyph % 16) * 24.0f);
            const float tex_y = ((c.glyph / 16) * 24.0f);
            const float tex_xf = (tex_x / texture_w);
            const float tex_yf = (tex_y / texture_h);

            // Floor
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});

            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

            vertices.emplace_back(v3d{X,        Z, Y});
            vertices.emplace_back(v3d{X,        Z, Y + vsize});
            vertices.emplace_back(v3d{X + vsize, Z, Y + vsize});
            vertices.emplace_back(v3d{X + vsize, Z, Y});

            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
        }

        void add_cube(const int &x, const int &y, const int &z, const rltk::vchar &c) {
            const float light_r = light_red(c);
            const float light_g = light_green(c);
            const float light_b = light_blue(c);

            const float X = (float)x;
            const float Y = (float)y;
            const float Z = (float)z;
            const float vsize = 1.0f;

            const float tex_x = (c.glyph % 16) * 24.0f;
            const float tex_y = (c.glyph / 16) * 24.0f;
            const float tex_xf = (float) tex_x / texture_w;
            const float tex_yf = (float) tex_y / texture_h;

            // Floor
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});

            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

            vertices.emplace_back(v3d{X,        Z, Y});
            vertices.emplace_back(v3d{X,        Z, Y + vsize});
            vertices.emplace_back(v3d{X + vsize, Z, Y + vsize});
            vertices.emplace_back(v3d{X + vsize, Z, Y});

            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});

            // Left Face
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});

            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

            vertices.emplace_back(v3d{X, Z, Y});
            vertices.emplace_back(v3d{X, Z, Y + vsize});
            vertices.emplace_back(v3d{X, Z + vsize, Y + vsize});
            vertices.emplace_back(v3d{X, Z + vsize, Y});

            normals.emplace_back(v3d{-1.0f, 0.0f, 0.0f});
            normals.emplace_back(v3d{-1.0f, 0.0f, 0.0f});
            normals.emplace_back(v3d{-1.0f, 0.0f, 0.0f});
            normals.emplace_back(v3d{-1.0f, 0.0f, 0.0f});

            // Right Face
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});

            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

            vertices.emplace_back(v3d{X + vsize, Z, Y});
            vertices.emplace_back(v3d{X + vsize, Z, Y + vsize});
            vertices.emplace_back(v3d{X + vsize, Z + vsize, Y + vsize});
            vertices.emplace_back(v3d{X + vsize, Z + vsize, Y});

            normals.emplace_back(v3d{1.0f, 0.0f, 0.0f});
            normals.emplace_back(v3d{1.0f, 0.0f, 0.0f});
            normals.emplace_back(v3d{1.0f, 0.0f, 0.0f});
            normals.emplace_back(v3d{1.0f, 0.0f, 0.0f});

            // North face
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});

            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

            vertices.emplace_back(v3d{X, Z, Y});
            vertices.emplace_back(v3d{X + vsize, Z, Y});
            vertices.emplace_back(v3d{X + vsize, Z + vsize, Y});
            vertices.emplace_back(v3d{X, Z + vsize, Y});

            normals.emplace_back(v3d{0.0f, 0.0f, -1.0f});
            normals.emplace_back(v3d{0.0f, 0.0f, -1.0f});
            normals.emplace_back(v3d{0.0f, 0.0f, -1.0f});
            normals.emplace_back(v3d{0.0f, 0.0f, -1.0f});

            // South face
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});

            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

            vertices.emplace_back(v3d{X, Z, Y + vsize});
            vertices.emplace_back(v3d{X + vsize, Z, Y + vsize});
            vertices.emplace_back(v3d{X + vsize, Z + vsize, Y + vsize});
            vertices.emplace_back(v3d{X, Z + vsize, Y + vsize});

            normals.emplace_back(v3d{0.0f, 0.0f, 1.0f});
            normals.emplace_back(v3d{0.0f, 0.0f, 1.0f});
            normals.emplace_back(v3d{0.0f, 0.0f, 1.0f});
            normals.emplace_back(v3d{0.0f, 0.0f, 1.0f});

            // Ceiling - facing up because that's what is visible
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});
            cvertices.emplace_back(c3d{light_r, light_g, light_b});

            tvertices.emplace_back(t3d{tex_xf, tex_yf});
            tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
            tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

            vertices.emplace_back(v3d{X,        Z+vsize, Y});
            vertices.emplace_back(v3d{X,        Z+vsize, Y + vsize});
            vertices.emplace_back(v3d{X + vsize, Z+vsize, Y + vsize});
            vertices.emplace_back(v3d{X + vsize, Z+vsize, Y});

            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
            normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
        }

        void render() const noexcept {
            //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(3, GL_FLOAT, 0, &cvertices[0]);

            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, &tvertices[0]);

            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, 0, &normals[0]);

            glDrawArrays(GL_QUADS, 0, vertices.size());

            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
        }

        void render_index() const noexcept {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(3, GL_FLOAT, 0, &screen_index[0]);

            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

            glDrawArrays(GL_QUADS, 0, vertices.size());

            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
    };

    render_block world_exterior_geometry;
    render_block world_interior_geometry;
    render_block floor_exterior_geometry;
    render_block floor_interior_geometry;
    std::unordered_map<int, render_block> game_lit_geometry;
    render_block mouse_picker_geometry;

    /*
     * Reset all buffers. Only used the first time, or when the world geometry changes.
     */
    void reset_all() {
        world_exterior_geometry.reset();
        world_interior_geometry.reset();
        floor_exterior_geometry.reset();
        floor_interior_geometry.reset();
        game_lit_geometry.clear();
    }

    // Adds a world-geometry floor tile
    void add_world_floor(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx) {
        auto light = lit_tiles.find(idx);
        if (light != lit_tiles.end()) {
            game_lit_geometry[light->second.first].add_floor(x, y, z, c);
        } else {
            if (current_region->above_ground[idx]) {
                floor_exterior_geometry.add_floor(x, y, z, c);
            } else {
                floor_interior_geometry.add_floor(x, y, z, c);
            }
        }
        mouse_picker_geometry.add_floor(x,y,z,c);
    }

    // Adds a world-geometry cube (solid)
    void add_world_cube(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx) {
        auto light = lit_tiles.find(idx);
        if (light != lit_tiles.end()) {
            game_lit_geometry[light->second.first].add_cube(x, y, z, c);
        } else {
            if (current_region->above_ground[idx]) {
                world_exterior_geometry.add_cube(x, y, z, c);
            } else {
                world_interior_geometry.add_cube(x, y, z, c);
            }
        }
        mouse_picker_geometry.add_floor(x,y,z,c);
    }

    bool is_daytime() noexcept {
        if (calendar->hour > 5 && calendar->hour < 18) {
            return true;
        } else {
            return false;
        }
    }

    void setup_sun_and_moon(const int &deferred_program) {
        const float latitude = ((float)current_region->region_y - ((float)WORLD_HEIGHT/2.0F)) * (float)REGION_HEIGHT;
        const auto pos = glGetUniformLocation(deferred_program, "light_position");
        const auto ambient = glGetUniformLocation(deferred_program, "light_ambient");
        const auto diffuse = glGetUniformLocation(deferred_program, "light_diffuse");

        if (is_daytime()) {
            // Sun
            const float time_as_decimal = ((float)calendar->hour-6.0f + ((float)calendar->minute / 60.0f)) / 12.0f;
            const float sun_angle_degrees = time_as_decimal * (float)REGION_WIDTH;

            GLfloat lightAmbient[3] = { 0.4f, 0.4f, 0.35f };
            GLfloat lightDiffuse[3] = { 1.0f, 1.0f, 0.9f };
            GLfloat lightPosition[3] = { sun_angle_degrees, 300.0f, latitude };

            glUniform3f(ambient, lightAmbient[0], lightAmbient[1], lightAmbient[2]);
            glUniform3f(diffuse, lightDiffuse[0], lightDiffuse[1], lightDiffuse[2]);
            glUniform3f(pos, lightPosition[0], lightDiffuse[1], lightDiffuse[2]);
        } else {
            // Moon
            // Set the light position
            GLfloat lightAmbient[3] = { 0.3f, 0.3f, 0.4f };
            GLfloat lightDiffuse[3] = { 0.7f, 0.7f, 0.8f };
            //GLfloat lightPosition[3] = { (float)camera_position->region_x-1.0f, (float)camera_position->region_z+20.0f, (float)camera_position->region_y-1.0f };
            GLfloat lightPosition[3] = { REGION_WIDTH/2.0f , 300.0f, -latitude };

            glUniform3f(ambient, lightAmbient[0], lightAmbient[1], lightAmbient[2]);
            glUniform3f(diffuse, lightDiffuse[0], lightDiffuse[1], lightDiffuse[2]);
            glUniform3f(pos, lightPosition[0], lightDiffuse[1], lightDiffuse[2]);
        }

    }

    void setup_indoor_lighting_unlit(const int &deferred_program) {
        const auto pos = glGetUniformLocation(deferred_program, "light_position");
        const auto ambient = glGetUniformLocation(deferred_program, "light_ambient");
        const auto diffuse = glGetUniformLocation(deferred_program, "light_diffuse");

        GLfloat lightAmbient[3] = { 0.3f, 0.3f, 0.4f };
        GLfloat lightDiffuse[3] = { 0.7f, 0.7f, 0.8f };
        //GLfloat lightPosition[3] = { (float)camera_position->region_x-1.0f, (float)camera_position->region_z+20.0f, (float)camera_position->region_y-1.0f };
        GLfloat lightPosition[3] = { 0.f, 0.f, 0.f };

        glUniform3f(ambient, lightAmbient[0], lightAmbient[1], lightAmbient[2]);
        glUniform3f(diffuse, lightDiffuse[0], lightDiffuse[1], lightDiffuse[2]);
        glUniform3f(pos, lightPosition[0], lightDiffuse[1], lightDiffuse[2]);
    }

    void setup_indoor_lighting_lit(const int &deferred_program, const std::pair<int, rltk::color_t> &light) {
        const auto pos = glGetUniformLocation(deferred_program, "light_position");
        const auto ambient = glGetUniformLocation(deferred_program, "light_ambient");
        const auto diffuse = glGetUniformLocation(deferred_program, "light_diffuse");

        GLfloat lightAmbient[3] = { 0.3f, 0.3f, 0.4f };
        GLfloat lightDiffuse[3] = { (float)light.second.r/255.0f, (float)light.second.g/255.0f, (float)light.second.b/255.0f };
        //GLfloat lightPosition[3] = { (float)camera_position->region_x-1.0f, (float)camera_position->region_z+20.0f, (float)camera_position->region_y-1.0f };
        int x,y,z;
        std::tie(x,y,z) = idxmap(light.first);
        GLfloat lightPosition[3] = { (float)x, (float)z+0.9f, (float)y };

        glUniform3f(ambient, lightAmbient[0], lightAmbient[1], lightAmbient[2]);
        glUniform3f(diffuse, lightDiffuse[0], lightDiffuse[1], lightDiffuse[2]);
        glUniform3f(pos, lightPosition[0], lightDiffuse[1], lightDiffuse[2]);
    }

    // Renders the world geometry
    void render_world(const GLuint &program_id, const GLuint &deferred_id)
    {
        glUseProgram(deferred_id);

        // Setup outdoor illumination
        glDisable(GL_LIGHTING);

        // Pass texture to the shader
        int texture_location = glGetUniformLocation(deferred_id, "my_color_texture");
        glUniform1i(texture_location, 0);

        // Call the rendering

        // Exterior - lit by the sun/moon
        setup_sun_and_moon(deferred_id);
        floor_exterior_geometry.render();
        world_exterior_geometry.render();

        // With game-provided lighting
        for (auto it=game_lit_geometry.begin(); it!=game_lit_geometry.end(); ++it) {
            setup_indoor_lighting_lit(deferred_id, lit_tiles[it->first]);
            it->second.render();
        }

        // Interior with no lights
        setup_indoor_lighting_unlit(deferred_id);
        world_interior_geometry.render();
        floor_interior_geometry.render();
        glUseProgram(0);
    }

    // Adds renderable vegetation
    void add_vegetation(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx) {
        auto light = lit_tiles.find(idx);
        if (light != lit_tiles.end()) {
            game_lit_geometry[light->second.first].add_decal(x, y, z, c);
        } else {
            floor_exterior_geometry.add_decal(x, y, z, c);
        }
    }

    // Adds decals (blood stains)
    void add_decal(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx) {
        auto light = lit_tiles.find(idx);
        if (light != lit_tiles.end()) {
            game_lit_geometry[light->second.first].add_decal(x, y, z, c);
        } else {
            if (current_region->above_ground[idx]) {
                floor_exterior_geometry.add_decal(x, y, z, c);
            } else {
                floor_interior_geometry.add_decal(x, y, z, c);
            }
        }
    }

    // Adds simple renderable
    void add_simple_renderable(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx) {
        auto light = lit_tiles.find(idx);
        if (light != lit_tiles.end()) {
            game_lit_geometry[light->second.first].add_decal(x, y, z, c);
        } else {
            if (current_region->above_ground[idx]) {
                floor_exterior_geometry.add_decal(x, y, z, c);
            } else {
                floor_interior_geometry.add_decal(x, y, z, c);
            }
        }
    }

    // Adds composite renderable
    void add_composite_renderable(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx) {
        auto light = lit_tiles.find(idx);
        if (light != lit_tiles.end()) {
            game_lit_geometry[light->second.first].add_floor(x, y, z, c);
        } else {
            if (current_region->above_ground[idx]) {
                floor_exterior_geometry.add_floor(x, y, z, c);
            } else {
                floor_interior_geometry.add_floor(x, y, z, c);
            }
        }
    }

    void render_index(const GLuint &program_id) {
        //glUseProgram(program_id);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR);

        mouse_picker_geometry.render_index();
        glUseProgram(0);
    }
}
