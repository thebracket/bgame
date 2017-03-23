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
#include "../../systems/input/mouse_input_system.hpp"
#include "render_block.hpp"

constexpr bool show_index_buffer = false;
bool world_changed = true;

namespace map_render {

    bool loaded_program = false;
    GLuint program_id;
    GLuint deferred_id;
    GLuint index_program_id;
    GLuint mouse_pick_texture;
    GLuint mouse_pick_fbo;
    GLuint mouse_pick_depth;

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

    inline void populate_vertex_buffer(const int &start_z, const int &end_z) {
        const vchar bloodstain{ 340, color_t{255,255,255}, color_t{255,255,255} };

        const int start_x = std::max(1, camera_position->region_x - 24);
        const int end_x = std::min(REGION_WIDTH-1, camera_position->region_x + 24);
        const int start_y = std::max(1, camera_position->region_y - 24);
        const int end_y = std::min(REGION_HEIGHT-1, camera_position->region_y + 24);

        for (int z=start_z; z<end_z; ++z) {
            for (int y = start_y; y < end_y; ++y) {
                for (int x = start_x; x < end_x; ++x) {
                    if (gl_frustrum::point_in_frustrum(x, z, y)) {
                        const int idx = mapidx(x, y, z);
                        if (current_region->revealed[idx]) {
                            const uint8_t tiletype = current_region->tile_type[idx];
                            if (tiletype != tile_type::OPEN_SPACE) {
                                if (tiletype == tile_type::RAMP) {
                                    world_scene::add_world_fractional_cube(x, y, z, current_region->render_cache[idx], idx, 0.5f);
                                } else if (has_ceiling(tiletype)) {
                                    if (game_master_mode == DESIGN && game_design_mode == CHOPPING && current_region->tree_id[idx]>0
                                        && designations->chopping.find(current_region->tree_id[idx])!=designations->chopping.end()) {
                                        world_scene::add_world_cube(x, y, z, vchar{'*', colors::RED, colors::RED}, idx);
                                    } else {
                                        world_scene::add_world_cube(x, y, z, current_region->render_cache[idx], idx);
                                    }
                                } else if (has_floor(tiletype)) {
                                    world_scene::add_world_floor(x, y, z, current_region->render_cache[idx], idx);
                                }

                                // Add water
                                const float water_depth = (float)current_region->water_level[idx] / 10.0f;
                                if (water_depth>0) {
                                    world_scene::add_world_fractional_cube(x, y, z, current_region->render_cache[idx], idx, water_depth);
                                }

                                // Add vegetation
                                auto veg_cache = current_region->veg_cache[idx];
                                if (veg_cache.glyph > 0) {
                                    world_scene::add_vegetation(x, y, z, veg_cache, idx);
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
    }

    inline void build_world_vertex_buffer(const float texture_w, const float texture_h) {
        world_scene::reset_all();

        // Add world geometry, depending upon game mode
        const bool single_z_layer = !(game_master_mode == PLAY || game_master_mode == ROGUE);

        if (single_z_layer) {
            populate_vertex_buffer(camera_position->region_z, camera_position->region_z+1);
        } else {
            populate_vertex_buffer(std::max(1, camera_position->region_z - 15),
                                   std::min(REGION_DEPTH - 1, camera_position->region_z + 1));
        }
    }

    void gl_states() {
        auto screen_size = rltk::get_window()->getSize();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT);
        glShadeModel(GL_SMOOTH);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glViewport(0,0,screen_size.x,screen_size.y);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(90.f, 1.f, 1.f, 300.0f);

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
    }

    inline void draw_world(const float texture_w, const float texture_h) {
        if (!loaded_program) {
            program_id = LoadShaders("world_defs/vertex.glsl", "world_defs/fragment.glsl");
            deferred_id = LoadShaders("world_defs/vertex-deferred.glsl", "world_defs/fragment-deferred.glsl");
            index_program_id = LoadShaders("world_defs/vertex-index.glsl", "world_defs/fragment-index.glsl");
            loaded_program = true;
            calculate_texture_info();

            // Create and bind the framebuffer for mouse-picking output
            glGenFramebuffers(1, &mouse_pick_fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);

            // Create the render target texture
            auto screen_size = rltk::get_window()->getSize();
            glGenTextures(1, &mouse_pick_texture);
            glBindTexture(GL_TEXTURE_2D, mouse_pick_texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_size.x, screen_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mouse_pick_texture, 0);

            // Create a depth-buffer for the render target
            glGenRenderbuffers(1, &mouse_pick_depth);
            glBindRenderbuffer(GL_RENDERBUFFER, mouse_pick_depth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_size.x, screen_size.y);
            glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mouse_pick_depth);

            // Configure the framebuffer
            //GLenum draw_buffers[2] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
            //glDrawBuffers(2, draw_buffers);

            glBindFramebuffer(GL_FRAMEBUFFER, 0); // Return to screen rendering
        }

        gl_frustrum::extract();
        build_world_vertex_buffer(texture_w, texture_h);

        // Render the main visible scene
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Return to screen rendering
        world_scene::render_world(program_id, deferred_id);

        // Render the framebuffer
        if (world_changed) {
            glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
            gl_states();
            glDisable(GL_TEXTURE_2D);
            world_scene::render_index(index_program_id);
            glBindFramebuffer(GL_FRAMEBUFFER, 0); // Return to screen rendering
            world_changed = false;
        }
    }

    std::tuple<int,int,int> readback_texture_pixel(const int &x, const int &y) {
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        uint8_t pixel[4] = {0,0,0,0};
        glReadPixels(x, get_window()->getSize().y - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel[0]);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        int mx = pixel[0];
        int my = pixel[1];
        int mz = pixel[2];
        //std::cout << "Mouse: " << x << "/" << y << " = " << mx << "/" << my << "/" << mz << "\n";
        return std::make_tuple(mx, my, mz);
        //return std::make_tuple(0, 0, 0);
    };

}

void map_render_t::render() {
    push_gl_states();

    map_render::gl_states();

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

    // Render the index buffer
    if (show_index_buffer) {
        push_gl_states();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(100, 100, 600, 600);
        glOrtho(100, 600, 100, 600, 0.0f, 1.0f);
        glEnableClientState(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, map_render::mouse_pick_texture);

        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);

        glTexCoord2f(0, 1);
        glVertex2f(100.0f, 600.0f);

        glTexCoord2f(0, 0);
        glVertex2f(100.0f, 100.0f);

        glTexCoord2f(1, 0);
        glVertex2f(600.0f, 100.0f);

        glTexCoord2f(1, 1);
        glVertex2f(600.0f, 600.0f);

        glEnd();
        pop_gl_states();
    }
}