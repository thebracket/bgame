#include "map_render.hpp"
#include "gl_utils.hpp"
#include <rltk.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <iostream>
#include "gl_utils.hpp"
#include "../../main/game_globals.hpp"
#include "../../systems/render/lighting_system.hpp"
#include "../../systems/render/renderables_system.hpp"
#include "../../systems/render/map_render_system.hpp"
#include "world_scene.hpp"
#include "frustrum.hpp"

bool world_changed = true;

namespace map_render {

    bool loaded_program = false;
    GLuint program_id;
    GLuint deferred_id;

    inline bool has_floor(uint8_t tiletype) {
        if (tiletype != tile_type::OPEN_SPACE) {
            return true;
        }
        return false;
    }

    inline bool has_ceiling(uint8_t tiletype) {
        if (tiletype == tile_type::TREE_TRUNK || tiletype == tile_type::WALL || tiletype == tile_type::SEMI_MOLTEN_ROCK
            || tiletype == tile_type::SOLID || tiletype == tile_type::TREE_LEAF || tiletype == tile_type::WINDOW ||
                tiletype == tile_type::CLOSED_DOOR) {
            return true;
        }
        return false;
    }

    inline void build_world_vertex_buffer(const float texture_w, const float texture_h) {
        // TODO: Break this into utilities, separate world geometry from renderables and vegetation
        // Basically, it's scene graph time.


        world_scene::reset_all();

        const vchar bloodstain{ 340, color_t{255,255,255}, color_t{255,255,255} };
        // This is a test cube - comment out unless fixing basic render!
        //world_scene::add_world_cube(camera_position->region_x, camera_position->region_y, camera_position->region_z, vchar{257, rltk::colors::WHITE, rltk::colors::BLACK}, mapidx(camera_position->region_x, camera_position->region_y, camera_position->region_z));

        // Add world geometry
        const int start_z = std::max(1, camera_position->region_z - 15);
        const int end_z = std::min(REGION_DEPTH-1, camera_position->region_z+1);
        for (int z=start_z; z<end_z; ++z) {
            for (int y = 0; y < REGION_HEIGHT; ++y) {
                for (int x = 0; x < REGION_WIDTH; ++x) {
                    if (gl_frustrum::point_in_frustrum(x, z, y) || gl_frustrum::point_in_frustrum(x-1.0f, z, y) || gl_frustrum::point_in_frustrum(x+1.0f, z, y)) {
                        const int idx = mapidx(x, y, z);
                        if (current_region->revealed[idx]) {
                            const uint8_t tiletype = current_region->tile_type[idx];
                            if (tiletype != tile_type::OPEN_SPACE) {
                                if (has_floor(tiletype)) {
                                    world_scene::add_world_floor(x, y, z, current_region->render_cache[idx], idx);
                                }
                                if (has_ceiling(tiletype)) {
                                    world_scene::add_world_cube(x, y, z, current_region->render_cache[idx], idx);
                                }

                                // Add vegetation
                                auto veg_cache = current_region->veg_cache[idx];
                                if (veg_cache.glyph > 0) {
                                    world_scene::add_vegetation(x, y, z, veg_cache);
                                }

                                // Add blood stains
                                if (current_region->blood_stains[idx]) {
                                    world_scene::add_decal(x, y, z, bloodstain, idx);
                                }

                                // Add renderables
                                auto rfinder = renderables.find(idx);
                                if (rfinder != renderables.end()) {
                                    screen_render_t &sr = rfinder->second[0];
                                    world_scene::add_simple_renderable(x, y, z, sr.c, idx);
                                }

                                // Add composite renderables
                                auto cfinder = composite_renderables.find(idx);
                                if (cfinder != composite_renderables.end()) {
                                    for (const auto &sr : cfinder->second[0]) {
                                        world_scene::add_composite_renderable(x, y, z, sr.c, idx);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        world_changed = false;
    }

    inline void draw_world(const float texture_w, const float texture_h) {
        if (!loaded_program) {
            program_id = LoadShaders("world_defs/vertex.glsl", "world_defs/fragment.glsl");
            deferred_id = LoadShaders("world_defs/vertex-deferred.glsl", "world_defs/fragment-deferred.glsl");
            loaded_program = true;
            world_scene::calculate_texture_info();
        }

        gl_frustrum::extract();
        build_world_vertex_buffer(texture_w, texture_h);
        world_scene::render_world(program_id, deferred_id);
    }

}

void map_render_t::render() {
    push_gl_states();
    const auto screen_size = rltk::get_window()->getSize();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glViewport(0,0,screen_size.x,screen_size.y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(90.f, (float)screen_size.x / (float)screen_size.y, 0.1f, 64.0f);//fov, aspect, zNear, zFar
    glOrtho(-12.0f, 12.0f, -12.0f, 12.0f, 0.0f, 128.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /*gluLookAt((float)camera_position->region_x, ((float)camera_position->region_z)+1.0f, ((float)camera_position->region_y)+0.1f, // Camera
              (float)camera_position->region_x, (float)camera_position->region_z, (float)camera_position->region_y, // Target
              0.0f, 1.0f, 0.0f // Up
    );*/

    gluLookAt((float)camera_position->region_x, ((float)camera_position->region_z)+12.0f, ((float)camera_position->region_y)+4.0f, // Camera
              (float)camera_position->region_x, (float)camera_position->region_z, (float)camera_position->region_y, // Target
              0.0f, 1.0f, 0.0f // Up
    );

    glEnable(GL_TEXTURE_2D);
    sf::Texture::bind(rltk::get_texture(term(layer_texture)->get_font_tag()));
    auto texture_size_vec = rltk::get_texture(term(layer_texture)->get_font_tag())->getSize();
    const float texture_w = texture_size_vec.x;
    const float texture_h = texture_size_vec.y;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    map_render::draw_world(texture_w, texture_h);

    pop_gl_states();
}