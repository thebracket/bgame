#include "world_scene.hpp"
#include "../vox/voxreader.hpp"
#include "../vox/voxel_model.hpp"
#include <vector>
#include <rltk.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include "../../systems/render/map_render_system.hpp"
#include "../../systems/render/lighting_system.hpp"
#include "render_block.hpp"
#include "../../systems/input/mouse_input_system.hpp"
#include "../../main/game_calendar.hpp"
#include "../../planet/region/region.hpp"

using namespace rltk;
using namespace region;

namespace world_scene {

    render_block world_exterior_geometry;
    render_block world_interior_geometry;
    render_block floor_exterior_geometry;
    render_block floor_interior_geometry;
    std::unordered_map<int, render_block> game_lit_geometry;

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

    bool is_daytime() noexcept {
        if (calendar->hour > 5 && calendar->hour < 18) {
            return true;
        } else {
            return false;
        }
    }

    // Adds a world-geometry floor tile
    void add_world_floor(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx) {
        auto light = lit_tiles.find(idx);
        if (light != lit_tiles.end() && ( !is_daytime() | !above_ground(idx) )) {
            game_lit_geometry[light->second.first].add_floor(x, y, z, c);
        } else {
            if (above_ground(idx)) {
                floor_exterior_geometry.add_floor(x, y, z, c);
            } else {
                floor_interior_geometry.add_floor(x, y, z, c);
            }
        }
        //if (z == camera_position->region_z) mouse_picker_geometry.add_floor(x,y,z,c);
    }

    // Adds a world-geometry cube (solid)
    void add_world_cube(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx) {
        auto light = lit_tiles.find(idx);
        if (light != lit_tiles.end()) {
            game_lit_geometry[light->second.first].add_cube(x, y, z, c);
        } else {
            if (above_ground(idx)) {
                world_exterior_geometry.add_cube(x, y, z, c);
            } else {
                world_interior_geometry.add_cube(x, y, z, c);
            }
        }
        //if (z == camera_position->region_z) mouse_picker_geometry.add_floor(x,y,z,c);
    }

    // Adds a world-geometry cube (solid)
    void add_world_fractional_cube(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx, const float &height) {
        auto light = lit_tiles.find(idx);
        if (light != lit_tiles.end()) {
            game_lit_geometry[light->second.first].add_fractional_height_cube(x, y, z, c, height);
        } else {
            if (above_ground(idx)) {
                world_exterior_geometry.add_fractional_height_cube(x, y, z, c, height);
            } else {
                world_interior_geometry.add_fractional_height_cube(x, y, z, c, height);
            }
        }
        //mouse_picker_geometry.add_floor(x,y,z,c);
    }

    void setup_sun_and_moon(const int &deferred_program) {
        const float latitude = (float)REGION_HEIGHT/2.0f + (((float)region_y() - ((float)WORLD_HEIGHT/2.0F)) * (float)REGION_HEIGHT);
        const auto pos = glGetUniformLocation(deferred_program, "light_position");
        const auto ambient = glGetUniformLocation(deferred_program, "light_ambient");
        const auto diffuse = glGetUniformLocation(deferred_program, "light_diffuse");

        float hour_sun_bias = (float)calendar->hour;
        const float minutes = ((float)calendar->minute / 60.0f)/10.0f;

        float time_of_day_radians = ((hour_sun_bias+minutes) * 3.141592653589793f) / 180.0f;
        std::pair<float, float> sun_pos = std::make_pair(
                (std::sin(time_of_day_radians)*256.0)+128.0f,
                (std::cos(time_of_day_radians)*256.0)+128.0f
        );
        //std::cout << time_of_day_radians << " = " << sun_pos.first << "," << sun_pos.second << "," << latitude << "\n";
        //std::cout << sun_pos.first << "\n";

        if (is_daytime()) {

            GLfloat lightAmbient[3] = { 0.4f, 0.4f, 0.35f };
            GLfloat lightDiffuse[3] = { 1.0f, 1.0f, 0.9f };
            GLfloat lightPosition[3] = { sun_pos.first, sun_pos.second, latitude };

            glUniform3f(ambient, lightAmbient[0], lightAmbient[1], lightAmbient[2]);
            glUniform3f(diffuse, lightDiffuse[0], lightDiffuse[1], lightDiffuse[2]);
            glUniform3f(pos, lightPosition[0], lightPosition[1], lightPosition[2]);
        } else {
            sun_pos.first = 0-sun_pos.first;
            sun_pos.second = 0-sun_pos.second;

            GLfloat lightAmbient[3] = { 0.3f, 0.3f, 0.5f };
            GLfloat lightDiffuse[3] = { 0.7f, 0.7f, 0.9f };
            GLfloat lightPosition[3] = { sun_pos.first, sun_pos.second, latitude };

            glUniform3f(ambient, lightAmbient[0], lightAmbient[1], lightAmbient[2]);
            glUniform3f(diffuse, lightDiffuse[0], lightDiffuse[1], lightDiffuse[2]);
            glUniform3f(pos, lightPosition[0], lightPosition[1], lightPosition[2]);
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
        glUniform3f(pos, lightPosition[0], lightPosition[1], lightPosition[2]);
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
        GLfloat lightPosition[3] = { (float)x+0.5f, (float)z+0.9f, (float)y+0.5f };

        glUniform3f(ambient, lightAmbient[0], lightAmbient[1], lightAmbient[2]);
        glUniform3f(diffuse, lightDiffuse[0], lightDiffuse[1], lightDiffuse[2]);
        glUniform3f(pos, lightPosition[0], lightPosition[1], lightPosition[2]);
    }

    // Renders the world geometry
    void render_world(const GLuint &deferred_id)
    {
        //glUseProgram(deferred_id);
        glPolygonMode( GL_FRONT, GL_FILL );

        // Setup outdoor illumination
        glDisable(GL_LIGHTING);

        // Pass texture to the shader
        int texture_location = glGetUniformLocation(deferred_id, "my_color_texture");
        glUniform1i(texture_location, 0);

        int projection_matrix_loc = glGetUniformLocation(deferred_id, "projection_matrix");
        float proj[16];
        glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&proj);
        glUniformMatrix4fv(projection_matrix_loc, 1, false, (GLfloat*)&proj);

        int view_matrix_loc = glGetUniformLocation(deferred_id, "view_matrix");
        float view[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&view);
        glUniformMatrix4fv(view_matrix_loc, 1, false, (GLfloat*)&view);

        // Call the rendering

        // Exterior - lit by the sun/moon
        setup_sun_and_moon(deferred_id);
        floor_exterior_geometry.render(deferred_id);
        world_exterior_geometry.render(deferred_id);

        // With game-provided lighting
        for (auto it=game_lit_geometry.begin(); it!=game_lit_geometry.end(); ++it) {
            setup_indoor_lighting_lit(deferred_id, lit_tiles[it->first]);
            it->second.render(deferred_id);
        }

        // Interior with no lights
        setup_indoor_lighting_unlit(deferred_id);
        world_interior_geometry.render(deferred_id);
        floor_interior_geometry.render(deferred_id);
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
            if (above_ground(idx)) {
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
            game_lit_geometry[light->second.first].add_floor(x, y, z, c);
        } else {
            if (above_ground(idx)) {
                floor_exterior_geometry.add_floor(x, y, z, c);
            } else {
                floor_interior_geometry.add_floor(x, y, z, c);
            }
        }
    }

    // Adds composite renderable
    void add_composite_renderable(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx) {
        auto light = lit_tiles.find(idx);
        if (light != lit_tiles.end()) {
            game_lit_geometry[light->second.first].add_standup(x, y, z, c, 1.0f);
        } else {
            if (above_ground(idx)) {
                floor_exterior_geometry.add_standup(x, y, z, c, 1.0f);
            } else {
                floor_interior_geometry.add_standup(x, y, z, c, 1.0f);
            }
        }
    }

    // Adds an ASCII tile with background
    void add_ascii_tile(const int &x, const int &y, const int &z, const rltk::vchar &c, const int &idx) {
        const vchar bg{219, c.background, c.background};
        const vchar fg{c.glyph, c.foreground, c.foreground};
        auto light = lit_tiles.find(idx);
        if (light != lit_tiles.end()) {
            game_lit_geometry[light->second.first].add_floor(x, y, z, bg);
            game_lit_geometry[light->second.first].add_floor(x, y, z, fg);
        } else {
            if (above_ground(idx)) {
                floor_exterior_geometry.add_floor(x, y, z, bg);
                floor_exterior_geometry.add_floor(x, y, z, fg);
            } else {
                floor_interior_geometry.add_floor(x, y, z, bg);
                floor_interior_geometry.add_floor(x, y, z, fg);
            }
        }
        //mouse_picker_geometry.add_floor(x,y,z,c);
    }

    void add_model_renderable(const int &x, const int &y, const int &z, const std::size_t &model_idx, const int &idx) {
        voxel_model * model = vox::get_model(model_idx);
        if (model) {
            render_block * target = nullptr;
            auto light = lit_tiles.find(idx);
            if (light != lit_tiles.end()) {
                target = &game_lit_geometry[light->second.first];
            } else {
                if (above_ground(idx)) {
                    target = &floor_exterior_geometry;
                } else {
                    target = &floor_interior_geometry;
                }
            }

            if (target) {
                const rltk::vchar rchar{ 219, colors::WHITE, colors::BLACK };
                for (const auto &voxel : model->voxels) {
                    //rltk::vchar rchar{ 219, color_t{ voxel.r * 255.0f, voxel.g*255.0f, voxel.b * 255.0f }, colors::BLACK };
                    const float X = voxel.x + (float)x;
                    const float Y = voxel.y + (float)y;
                    const float Z = voxel.z + (float)z;
                    target->add_minicube(X, Y, Z, rchar);
                }
            }
        }
    }
}
