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
#include "../../messages/messages.hpp"
#include "../../systems/gui/mode_design_system.hpp"
#include "../../main/game_clipping.hpp"

using namespace map_render_sys;

bool world_changed = true;

namespace map_render {

    bool loaded_program = false;
    GLuint deferred_id;
    GLuint mouse_pick_texture;
    GLuint mouse_pick_fbo;
    GLuint mouse_pick_depth;
    GLuint render_texture;

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

    inline std::tuple<int, int, int, int> calculate_edges() noexcept {
        const int z2 = camera->zoom_level * 2;
        return std::make_tuple(
                std::max(1, camera_position->region_x - z2),
                std::min(REGION_WIDTH-1, camera_position->region_x + z2),
                std::max(1, camera_position->region_y - z2),
                std::min(REGION_HEIGHT-1, camera_position->region_y + z2)
        );
    }

    template <typename F>
    inline void render_area_loop(const int &start_z, const int &end_z, const std::tuple<int, int, int, int> &edges, const F &&func) {
        int start_x, end_x, start_y, end_y;
        std::tie(start_x, end_x, start_y, end_y) = calculate_edges();

        for (int z=start_z; z<end_z; ++z) {
            for (int y = start_y; y < end_y; ++y) {
                for (int x = start_x; x < end_x; ++x) {
                    if (gl_frustrum::point_in_frustrum(x, z, y)) {
                        const int idx = mapidx(x, y, z);
                        func(x, y, z, idx, current_region->revealed[idx], current_region->tile_type[idx]);
                    }
                }
            }
        }
    }

    const vchar bloodstain{ 340, color_t{255,255,255}, color_t{255,255,255} };

    void populate_tile_render(const int &x, const int &y, const int &z, const int &idx, const bool &revealed, const uint8_t &tiletype) {
        if (revealed) {
            if (tiletype != tile_type::OPEN_SPACE) {
                if (tiletype == tile_type::RAMP) {
                    world_scene::add_world_fractional_cube(x, y, z, current_region->render_cache[idx], idx, 0.5f);
                } else if (has_ceiling(tiletype)) {
                    if (game_master_mode == DESIGN && game_design_mode == CHOPPING && current_region->tree_id[idx]>0
                        && designations->chopping.find(current_region->tree_id[idx])!=designations->chopping.end()) {
                        world_scene::add_world_cube(x, y, z, vchar{'*', colors::RED, colors::RED}, idx);
                    } else {
                        if (tiletype == tile_type::TREE_TRUNK) {
                            world_scene::add_composite_renderable(x, y, z, current_region->render_cache[idx], idx);
                        } else {
                            world_scene::add_world_cube(x, y, z, current_region->render_cache[idx], idx);
                        }
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
                    screen_render_t &sr = rfinder->second[glyph_cycle % rfinder->second.size()];
                    world_scene::add_simple_renderable(x, y, z, sr.c, idx);
                }

                // Add composite renderables
                auto cfinder = composite_renderables.find(idx);
                if (cfinder != composite_renderables.end()) {
                    for (const auto &sr : cfinder->second[glyph_cycle % cfinder->second.size()]) {
                        world_scene::add_composite_renderable(x, y, z, sr.c, idx);
                    }
                }

                // Stockpiles
                if (game_master_mode == DESIGN && game_design_mode == STOCKPILES && current_stockpile>0 && current_region->stockpile_id[idx]==current_stockpile) {
                    world_scene::add_decal(x, y, z, vchar{'#', colors::MAGENTA, colors::MAGENTA}, idx);
                }

                // Mining
                if (game_master_mode == DESIGN && game_design_mode == DIGGING) {
                    auto mf = designations->mining.find(idx);
                    if (mf != designations->mining.end()) {
                        vchar result{'!', colors::YELLOW, colors::YELLOW};
                        switch (mf->second) {
                            case 1 : result.glyph = 177; break;
                            case 2 : result.glyph = 31; break;
                            case 3 : result.glyph = 30; break;
                            case 4 : result.glyph = '<'; break;
                            case 5 : result.glyph = '>'; break;
                            case 6 : result.glyph = 'X'; break;
                        }

                        if (mf->second != 5) {
                            world_scene::add_world_cube(x, y, z, result, idx);
                        } else {
                            world_scene::add_decal(x, y, z, result, idx);
                        }
                    }
                }

                // Buildings
                if (game_master_mode == DESIGN && game_design_mode == BUILDING && has_build_mode_building) {
                    const int building_left_x = mouse::mouse_world_x;
                    const int building_top_y = mouse::mouse_world_y;
                    const int building_right_x = mouse::mouse_world_x + build_mode_building.width;
                    const int building_bottom_y = mouse::mouse_world_y + build_mode_building.height;

                    if (x >= building_left_x && x < building_right_x && y >= building_top_y && y < building_bottom_y) {

                        vchar result{'*', colors::BLACK, colors::BLACK};
                        if ((!current_region->solid[idx] &&
                             current_region->tile_flags[idx].test(CAN_STAND_HERE)
                             && !current_region->tile_flags[idx].test(CONSTRUCTION)
                             && !(current_region->tile_type[idx] == tile_type::STAIRS_DOWN)
                             && !(current_region->tile_type[idx] == tile_type::STAIRS_UP)
                             && !(current_region->tile_type[idx] == tile_type::STAIRS_UPDOWN))
                            || (build_mode_building.tag == "floor"
                                && !current_region->tile_flags[idx].test(CONSTRUCTION) &&
                                !(current_region->tile_type[idx] == tile_type::STAIRS_DOWN)
                                && !(current_region->tile_type[idx] == tile_type::STAIRS_UP)
                                && !(current_region->tile_type[idx] == tile_type::STAIRS_UPDOWN)
                                && (current_region->tile_flags[idx].test(CAN_STAND_HERE) ||
                                    current_region->tile_flags[idx - 1].test(CAN_STAND_HERE) ||
                                    current_region->tile_flags[idx + 1].test(CAN_STAND_HERE) ||
                                    current_region->tile_flags[idx - REGION_WIDTH].test(
                                            CAN_STAND_HERE) ||
                                    current_region->tile_flags[idx + REGION_WIDTH].test(CAN_STAND_HERE)
                                )
                            )
                                ) {
                            result.foreground = rltk::colors::GREEN;
                        } else {
                            result.foreground = rltk::colors::RED;
                            building_possible = false;
                        }
                        world_scene::add_decal(x, y, z, result, idx);
                    }
                }

                // Architecure Mode
                if (game_master_mode == DESIGN && game_design_mode == ARCHITECTURE) {
                    vchar result{' ', colors::GREY, colors::GREY};

                    // Display existing architecture
                    auto mf = designations->architecture.find(idx);
                    if (mf != designations->architecture.end()) {
                        result.foreground = rltk::colors::YELLOW;
                        result.background = rltk::colors::GREY;
                        switch (mf->second) {
                            case 0 : result.glyph = '#'; break;
                            case 1 : result.glyph = '.'; break;
                            case 2 : result.glyph = '<'; break;
                            case 3 : result.glyph = '>'; break;
                            case 4 : result.glyph = 'X'; break;
                            case 5 : result.glyph = '^'; break;
                            case 6 : result.glyph = '='; break;
                        }
                    }

                    if (arch_available && mf == designations->architecture.end()) {
                        const int building_left_x = mouse::mouse_world_x;
                        const int building_top_y = mouse::mouse_world_y;
                        const int building_right_x = mouse::mouse_world_x + arch_width;
                        const int building_bottom_y = mouse::mouse_world_y + arch_height;

                        if (arch_filled) {

                            if (x >= building_left_x && x < building_right_x &&
                                y >= building_top_y && y < building_bottom_y) {
                                result.background = rltk::colors::BLACK;
                                result.glyph = 177;

                                if (arch_possible && !current_region->solid[idx] &&
                                    !current_region->tile_flags[idx].test(CONSTRUCTION) && current_region->bridge_id[idx]==0) {
                                    result.foreground = rltk::colors::GREEN;
                                } else {
                                    arch_possible = false;
                                    result.foreground = rltk::colors::RED;
                                }
                            }
                        } else {
                            if (x >= building_left_x && x < building_right_x &&
                                y >= building_top_y && y < building_bottom_y) {
                                bool interior = false;
                                if (arch_width > 2 && x+clip_left >= building_left_x+1 && x+clip_left < building_right_x-1 ) {
                                    interior = true;
                                }
                                if (arch_height > 2 && y+clip_top >= building_top_y+1 && y+clip_top < building_bottom_y-1) {
                                    interior = true;
                                }
                                if (x+clip_left == building_left_x) interior=false;
                                if (x+clip_left == building_right_x-1) interior = false;
                                if (y+clip_top == building_top_y) interior = false;
                                if (y+clip_top == building_bottom_y-1) interior = false;

                                if (!interior) {
                                    result.background = rltk::colors::BLACK;
                                    result.glyph = 177;

                                    if (arch_possible && !current_region->solid[idx] &&
                                        !current_region->tile_flags[idx].test(CONSTRUCTION) && current_region->bridge_id[idx]==0) {
                                        result.foreground = rltk::colors::GREEN;
                                    } else {
                                        arch_possible = false;
                                        result.foreground = rltk::colors::RED;
                                    }
                                }
                            }
                        }
                    }

                    if (result.glyph != ' ') {
                        world_scene::add_world_cube(x, y, z, result, idx);
                    }
                }
            } else {
                // Add renderables in open space
                auto rfinder = renderables.find(idx);
                if (rfinder != renderables.end()) {
                    screen_render_t &sr = rfinder->second[glyph_cycle % rfinder->second.size()];
                    world_scene::add_simple_renderable(x, y, z, sr.c, idx);
                }

                // Add composite renderables in open space
                auto cfinder = composite_renderables.find(idx);
                if (cfinder != composite_renderables.end()) {
                    for (const auto &sr : cfinder->second[glyph_cycle % cfinder->second.size()]) {
                        world_scene::add_composite_renderable(x, y, z, sr.c, idx);
                    }
                }
            }
        } else if (game_master_mode == DESIGN && game_design_mode == DIGGING) {
            const uint8_t tiletype = current_region->tile_type[idx];
            if (tiletype != tile_type::OPEN_SPACE) {
                vchar result{257, colors::CYAN, colors::GREY};
                auto mf = designations->mining.find(idx);
                if (mf != designations->mining.end()) {
                    result.foreground = colors::YELLOW;
                    switch (mf->second) {
                        case 1 :
                            result.glyph = 177;
                            break;
                        case 2 :
                            result.glyph = 31;
                            break;
                        case 3 :
                            result.glyph = 30;
                            break;
                        case 4 :
                            result.glyph = '<';
                            break;
                        case 5 :
                            result.glyph = '>';
                            break;
                        case 6 :
                            result.glyph = 'X';
                            break;
                    }

                }
                world_scene::add_world_cube(x, y, z, result, idx);
                world_changed = true;
            }
        }
    }

    void populate_ascii_render(const int &x, const int &y, const int &z, const int &idx, const bool &revealed, const uint8_t &tiletype) {
        if (revealed) {
            rltk::vchar tile{' ', colors::BLACK, colors::BLACK};

            if (tiletype != tile_type::OPEN_SPACE) {
                if (tiletype == tile_type::RAMP) {
                    tile = current_region->render_cache[idx];
                } else if (has_ceiling(tiletype)) {
                    if (game_master_mode == DESIGN && game_design_mode == CHOPPING && current_region->tree_id[idx]>0
                        && designations->chopping.find(current_region->tree_id[idx])!=designations->chopping.end())
                    {
                        tile.glyph = '*';
                        tile.foreground = colors::RED;
                    } else {
                        tile = current_region->render_cache[idx];
                    }
                } else if (has_floor(tiletype)) {
                    tile = current_region->render_cache[idx];
                }

                // Add water
                const float water_depth = (float)current_region->water_level[idx] / 10.0f;
                if (water_depth>0) {
                    tile = current_region->render_cache[idx];
                }

                // Add vegetation
                auto veg_cache = current_region->veg_cache[idx];
                if (veg_cache.glyph > 0) {
                    tile = veg_cache;
                    tile.background = rltk::colors::BLACK;
                }

                // Add blood stains
                if (current_region->blood_stains[idx]) {
                    tile.background = colors::RED;
                }

                // Add renderables
                auto rfinder = renderables.find(idx);
                if (rfinder != renderables.end()) {
                    screen_render_t &sr = rfinder->second[glyph_cycle % rfinder->second.size()];
                    tile = sr.c;
                }

                // Add composite renderables
                auto cfinder = composite_renderables.find(idx);
                if (cfinder != composite_renderables.end()) {
                    tile = cfinder->second[glyph_cycle % cfinder->second.size()][0].c;
                }

                // Stockpiles
                if (game_master_mode == DESIGN && game_design_mode == STOCKPILES && current_stockpile>0 && current_region->stockpile_id[idx]==current_stockpile) {
                    tile = vchar{'#', colors::MAGENTA, colors::MAGENTA};
                }

                // Mining
                if (game_master_mode == DESIGN && game_design_mode == DIGGING) {
                    auto mf = designations->mining.find(idx);
                    if (mf != designations->mining.end()) {
                        vchar result{'!', colors::YELLOW, colors::YELLOW};
                        switch (mf->second) {
                            case 1 : result.glyph = 177; break;
                            case 2 : result.glyph = 31; break;
                            case 3 : result.glyph = 30; break;
                            case 4 : result.glyph = '<'; break;
                            case 5 : result.glyph = '>'; break;
                            case 6 : result.glyph = 'X'; break;
                        }

                        tile = result;
                    }
                }

                // Buildings
                if (game_master_mode == DESIGN && game_design_mode == BUILDING && has_build_mode_building) {
                    const int building_left_x = mouse::mouse_world_x;
                    const int building_top_y = mouse::mouse_world_y;
                    const int building_right_x = mouse::mouse_world_x + build_mode_building.width;
                    const int building_bottom_y = mouse::mouse_world_y + build_mode_building.height;

                    if (x >= building_left_x && x < building_right_x && y >= building_top_y && y < building_bottom_y) {

                        vchar result{'*', colors::BLACK, colors::BLACK};
                        if ((!current_region->solid[idx] &&
                             current_region->tile_flags[idx].test(CAN_STAND_HERE)
                             && !current_region->tile_flags[idx].test(CONSTRUCTION)
                             && !(current_region->tile_type[idx] == tile_type::STAIRS_DOWN)
                             && !(current_region->tile_type[idx] == tile_type::STAIRS_UP)
                             && !(current_region->tile_type[idx] == tile_type::STAIRS_UPDOWN))
                            || (build_mode_building.tag == "floor"
                                && !current_region->tile_flags[idx].test(CONSTRUCTION) &&
                                !(current_region->tile_type[idx] == tile_type::STAIRS_DOWN)
                                && !(current_region->tile_type[idx] == tile_type::STAIRS_UP)
                                && !(current_region->tile_type[idx] == tile_type::STAIRS_UPDOWN)
                                && (current_region->tile_flags[idx].test(CAN_STAND_HERE) ||
                                    current_region->tile_flags[idx - 1].test(CAN_STAND_HERE) ||
                                    current_region->tile_flags[idx + 1].test(CAN_STAND_HERE) ||
                                    current_region->tile_flags[idx - REGION_WIDTH].test(
                                            CAN_STAND_HERE) ||
                                    current_region->tile_flags[idx + REGION_WIDTH].test(CAN_STAND_HERE)
                                )
                            )
                                ) {
                            result.foreground = rltk::colors::GREEN;
                        } else {
                            result.foreground = rltk::colors::RED;
                            building_possible = false;
                        }
                        tile = result;
                    }
                }

                // Architecure Mode
                if (game_master_mode == DESIGN && game_design_mode == ARCHITECTURE) {
                    vchar result{' ', colors::GREY, colors::GREY};

                    // Display existing architecture
                    auto mf = designations->architecture.find(idx);
                    if (mf != designations->architecture.end()) {
                        result.foreground = rltk::colors::YELLOW;
                        result.background = rltk::colors::GREY;
                        switch (mf->second) {
                            case 0 : result.glyph = '#'; break;
                            case 1 : result.glyph = '.'; break;
                            case 2 : result.glyph = '<'; break;
                            case 3 : result.glyph = '>'; break;
                            case 4 : result.glyph = 'X'; break;
                            case 5 : result.glyph = '^'; break;
                            case 6 : result.glyph = '='; break;
                        }
                    }

                    if (arch_available && mf == designations->architecture.end()) {
                        const int building_left_x = mouse::mouse_world_x;
                        const int building_top_y = mouse::mouse_world_y;
                        const int building_right_x = mouse::mouse_world_x + arch_width;
                        const int building_bottom_y = mouse::mouse_world_y + arch_height;

                        if (arch_filled) {

                            if (x >= building_left_x && x < building_right_x &&
                                y >= building_top_y && y < building_bottom_y) {
                                result.background = rltk::colors::BLACK;
                                result.glyph = 177;

                                if (arch_possible && !current_region->solid[idx] &&
                                    !current_region->tile_flags[idx].test(CONSTRUCTION) && current_region->bridge_id[idx]==0) {
                                    result.foreground = rltk::colors::GREEN;
                                } else {
                                    arch_possible = false;
                                    result.foreground = rltk::colors::RED;
                                }
                            }
                        } else {
                            if (x >= building_left_x && x < building_right_x &&
                                y >= building_top_y && y < building_bottom_y) {
                                bool interior = false;
                                if (arch_width > 2 && x+clip_left >= building_left_x+1 && x+clip_left < building_right_x-1 ) {
                                    interior = true;
                                }
                                if (arch_height > 2 && y+clip_top >= building_top_y+1 && y+clip_top < building_bottom_y-1) {
                                    interior = true;
                                }
                                if (x+clip_left == building_left_x) interior=false;
                                if (x+clip_left == building_right_x-1) interior = false;
                                if (y+clip_top == building_top_y) interior = false;
                                if (y+clip_top == building_bottom_y-1) interior = false;

                                if (!interior) {
                                    result.background = rltk::colors::BLACK;
                                    result.glyph = 177;

                                    if (arch_possible && !current_region->solid[idx] &&
                                        !current_region->tile_flags[idx].test(CONSTRUCTION) && current_region->bridge_id[idx]==0) {
                                        result.foreground = rltk::colors::GREEN;
                                    } else {
                                        arch_possible = false;
                                        result.foreground = rltk::colors::RED;
                                    }
                                }
                            }
                        }
                    }

                    if (result.glyph != ' ') {
                        tile = result;
                    }
                }
                world_scene::add_ascii_tile(x, y, z, tile, idx);
            } else {
                // Add renderables
                auto rfinder = renderables.find(idx);
                if (rfinder != renderables.end()) {
                    screen_render_t &sr = rfinder->second[glyph_cycle % rfinder->second.size()];
                    tile = sr.c;
                }

                // Add composite renderables
                auto cfinder = composite_renderables.find(idx);
                if (cfinder != composite_renderables.end()) {
                    tile = cfinder->second[glyph_cycle % cfinder->second.size()][0].c;
                }
            } // end open space
        } else if (game_master_mode == DESIGN && game_design_mode == DIGGING) {
            const uint8_t tiletype = current_region->tile_type[idx];
            if (tiletype != tile_type::OPEN_SPACE) {
                vchar result{257, colors::CYAN, colors::GREY};
                auto mf = designations->mining.find(idx);
                if (mf != designations->mining.end()) {
                    result.foreground = colors::YELLOW;
                    switch (mf->second) {
                        case 1 :
                            result.glyph = 177;
                            break;
                        case 2 :
                            result.glyph = 31;
                            break;
                        case 3 :
                            result.glyph = 30;
                            break;
                        case 4 :
                            result.glyph = '<';
                            break;
                        case 5 :
                            result.glyph = '>';
                            break;
                        case 6 :
                            result.glyph = 'X';
                            break;
                    }

                }
                world_scene::add_ascii_tile(x, y, z, result, idx);
                world_changed = true;
            }
        }
    }

    inline void populate_vertex_buffer(const int &start_z, const int &end_z) {
        const auto edges = calculate_edges();
        if (camera->ascii_mode) {
            render_area_loop(start_z, end_z, edges, populate_ascii_render);
        } else {
            render_area_loop(start_z, end_z, edges, populate_tile_render);
        }
    }

    inline void build_world_vertex_buffer() {
        world_scene::reset_all();

        // Add world geometry, depending upon game mode
        const bool single_z_layer = !(game_master_mode == PLAY || game_master_mode == ROGUE);

        if (game_master_mode == DESIGN && game_design_mode == BUILDING && has_build_mode_building) building_possible = true;

        if (single_z_layer) {
            populate_vertex_buffer(camera_position->region_z, camera_position->region_z + 1);
        } else {
            populate_vertex_buffer(std::max(1, camera_position->region_z - 7),
                                         std::min(REGION_DEPTH - 1, camera_position->region_z + 1));
        }

        // Add marker types in design mode as needed
        if (game_master_mode == DESIGN) {
            if (game_design_mode == GUARDPOINTS) {
                for (const auto &gp : designations->guard_points) {
                    world_scene::add_decal(gp.second.x, gp.second.y, gp.second.z,
                                           vchar{'G', colors::CYAN, colors::CYAN}, mapidx(gp.second));
                }
            } else if (game_design_mode == HARVEST) {
                for (const auto &h : designations->harvest) {
                    world_scene::add_decal(h.second.x, h.second.y, h.second.z,
                                           vchar{'H', colors::CYAN, colors::CYAN}, mapidx(h.second));
                }
            }
        }

        if (game_master_mode == DESIGN && game_design_mode == BUILDING) {
            if (mouse::clicked) {
                emit(build_request_message{mouse::mouse_world_x, mouse::mouse_world_y, camera_position->region_z, build_mode_building});
                emit(refresh_available_buildings_message{});
                emit(map_dirty_message{});
                emit(renderables_changed_message{});
            }
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

        switch (camera->camera_mode) {
            case FRONT : {
                // Nice X-perspective view
                gluLookAt((float) camera_position->region_x, ((float) camera_position->region_z) + (float)camera->zoom_level,
                          ((float) camera_position->region_y) + ((float)camera->zoom_level/3.0f), // Camera
                          (float) camera_position->region_x, (float) camera_position->region_z,
                          (float) camera_position->region_y, // Target
                          0.0f, 1.0f, 0.0f // Up
                );
            } break;

            case TOP_DOWN : {
                // Top-down
                gluLookAt((float) camera_position->region_x, ((float) camera_position->region_z) + (float)camera->zoom_level,
                          ((float) camera_position->region_y) + 0.1f, // Camera
                          (float) camera_position->region_x, (float) camera_position->region_z,
                          (float) camera_position->region_y, // Target
                          0.0f, 1.0f, 0.0f // Up
                );
            } break;

            case DIAGONAL : {
                // Diagonal
                gluLookAt((float) camera_position->region_x + (float)camera->zoom_level, ((float) camera_position->region_z) + (float)camera->zoom_level,
                          ((float) camera_position->region_y) + (float)camera->zoom_level, // Camera
                          (float) camera_position->region_x, (float) camera_position->region_z,
                          (float) camera_position->region_y, // Target
                          0.0f, 1.0f, 0.0f // Up
                );
            } break;
        }

        glEnable(GL_TEXTURE_2D);
        //glActiveTexture(GL_TEXTURE0);
        sf::Texture::bind(rltk::get_texture(term(1)->get_font_tag()));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        /*glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, map_render::render_texture);
        glEnable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, map_render::mouse_pick_texture);
        glEnable(GL_TEXTURE_2D);*/

        GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
        glDrawBuffers(2, buffers);
    }

    inline void setup_fbo() {
        //program_id = LoadShaders("world_defs/vertex.glsl", "world_defs/fragment.glsl");
        deferred_id = LoadShaders("world_defs/vertex-deferred.glsl", "world_defs/fragment-deferred.glsl");
        //index_program_id = LoadShaders("world_defs/vertex-index.glsl", "world_defs/fragment-index.glsl");
        loaded_program = true;
        calculate_texture_info();

        // Create and bind the framebuffer for mouse-picking output
        glGenFramebuffers(1, &mouse_pick_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);

        // Create the mouse picker render target texture
        const auto screen_size = rltk::get_window()->getSize();
        glGenTextures(1, &mouse_pick_texture);
        glBindTexture(GL_TEXTURE_2D, mouse_pick_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_size.x, screen_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mouse_pick_texture, 0);

        // Create the render target texture
        glGenTextures(1, &render_texture);
        glBindTexture(GL_TEXTURE_2D, render_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_size.x, screen_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, render_texture, 0);

        // Create a depth-buffer for the render target
        glGenRenderbuffers(1, &mouse_pick_depth);
        glBindRenderbuffer(GL_RENDERBUFFER, mouse_pick_depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_size.x, screen_size.y);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mouse_pick_depth);

        GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
        glDrawBuffers(2, buffers);

        // Return to regular render mode
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    inline void draw_world() {
        if (!loaded_program) {
            setup_fbo();
        }

        gl_frustrum::extract();
        build_world_vertex_buffer();

        // Render the main visible scene
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gl_states();
        world_scene::render_world(deferred_id);
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Return to screen rendering

        /*
        // Render the framebuffer
        if (world_changed) {
            glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            gl_states();
            glDisable(GL_TEXTURE_2D);
            world_scene::render_index(index_program_id);
            glBindFramebuffer(GL_FRAMEBUFFER, 0); // Return to screen rendering
            world_changed = false;
        }*/
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

    glUseProgram(map_render::deferred_id);

    map_render::gl_states();

    map_render::draw_world();

    pop_gl_states();

    // Render the index buffer
    auto sz = rltk::get_window()->getSize();
    const float W = (float)sz.x;
    const float H = (float)sz.y;
    push_gl_states();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, sz.x, sz.y);
    glOrtho(0, sz.x, 0, sz.y, 0.0f, 1.0f);
    glEnableClientState(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, map_render::render_texture);

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 1);
    glVertex2f(0.0f, H);

    glTexCoord2f(0, 0);
    glVertex2f(0.0f, 0.0f);

    glTexCoord2f(1, 0);
    glVertex2f(W, 0.0f);

    glTexCoord2f(1, 1);
    glVertex2f(W, H);

    glEnd();
    pop_gl_states();
}