#pragma once

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif
#include "../../planet/constants.hpp"
#include "render_block.hpp"
#include "../../planet/indices.hpp"
#include "../../planet/region/region.hpp"
#include "../../raws/materials.hpp"
#include <array>

#include <boost/container/flat_map.hpp>
#include "../../raws/defs/material_def_t.hpp"
#include "../../systems/render/lighting_system.hpp"

namespace gl {

    constexpr int CHUNK_SIZE = 16;
    constexpr int CHUNKS_WIDTH = REGION_WIDTH / CHUNK_SIZE;
    constexpr int CHUNKS_HEIGHT = REGION_HEIGHT / CHUNK_SIZE;
    constexpr int CHUNKS_DEPTH = REGION_DEPTH / CHUNK_SIZE;
    constexpr int CHUNKS_TOTAL = CHUNKS_DEPTH * CHUNKS_HEIGHT * CHUNKS_WIDTH;

    constexpr int chunk_idx(const int x, const int y, const int z) {
        return (z * CHUNKS_HEIGHT * CHUNKS_WIDTH) + (y * CHUNKS_WIDTH) + x;
    }

    // Forward
    namespace geometry {
        constexpr float texture_width = 512.0f;
        constexpr float texture_height = 4096.0f;
        constexpr float sprite_width = 256.0f;
        constexpr float sprite_height = 256.0f;
        constexpr float tsize_x = sprite_width / texture_width;
        constexpr float tsize_y = sprite_height / texture_height;

        struct chunk_geometry_t {
            /*
             * Layout:
             * 0,1,2 = Vertex Position
             * 3,4,5 = World Position
             * 6,7,8 = Normal
             * 9,10,11 = Color
             * 12,13 = Texture Position
             * 14,15,16 = Flags
             * 17,18,19 = Light position
             * 20,21,22 = Light color
             */
            std::vector<float> items;
            int n_quads = 0;

            template<typename T> inline T add_to_items_impl(T arg) { items.emplace_back(arg); return arg; }

            template<typename... T>
            inline void add_to_items(T... args) {
                float float_array[] = { add_to_items_impl(args)... };
            }

            // Warning: this modifies parameters!
            inline void colorize_and_texture_floor(const int &idx, float &r, float &g, float &b, float &ty) {
                const uint8_t type = region::tile_type(idx);
                if (type == tile_type::TREE_TRUNK) {
                    r = 1.0f;
                    g = 1.0f;
                    b = 1.0f;
                    ty = 8.0f * sprite_height;
                } else {
                    const size_t material_idx = region::material(idx);
                    auto material_definition = get_material(material_idx);
                    if (material_definition != nullptr) {
                        if (region::flag(idx, CONSTRUCTION)) {
                            ty = sprite_height * material_definition->constructed_floor_texture;
                            r = (float) material_definition->fg.r / 255.0f;
                            g = (float) material_definition->fg.g / 255.0f;
                            b = (float) material_definition->fg.b / 255.0f;
                        } else {
                            ty = sprite_height * material_definition->floor_texture;
                            r = (float) material_definition->fg.r / 255.0f;
                            g = (float) material_definition->fg.g / 255.0f;
                            b = (float) material_definition->fg.b / 255.0f;
                        }
                    }
                }
                ty = ty / texture_height;
            }

            // Warning: this modifies parameters!
            inline void colorize_and_texture_other(const int &idx, float &r, float &g, float &b, float &ty) {
                const uint8_t type = region::tile_type(idx);
                if (type == tile_type::TREE_TRUNK) {
                    r = 1.0f;
                    g = 1.0f;
                    b = 1.0f;
                    ty = 8.0f * sprite_height;
                } else {
                    const size_t material_idx = region::material(idx);
                    auto material_definition = get_material(material_idx);
                    if (material_definition != nullptr) {
                        if (region::flag(idx, CONSTRUCTION)) {
                            ty = sprite_height * material_definition->constructed_wall_texture;
                            r = (float) material_definition->fg.r / 255.0f;
                            g = (float) material_definition->fg.g / 255.0f;
                            b = (float) material_definition->fg.b / 255.0f;
                        } else {
                            ty = sprite_height * material_definition->wall_texture;
                            r = (float) material_definition->fg.r / 255.0f;
                            g = (float) material_definition->fg.g / 255.0f;
                            b = (float) material_definition->fg.b / 255.0f;
                        }
                    }
                }
                ty = ty / texture_height;
            }

            void add_floor(const float x, const float y, const float z, float r, float g, float b, float tx,
                           float ty, const int &idx, const bool &above_ground,
                           const float &light_r, const float &light_g, const float &light_b,
                           const float &light_x, const float &light_y, const float &light_z)
            {
                colorize_and_texture_floor(idx, r, g, b, ty);
                const float ground_indicator = above_ground ? 255.0f : 0.0f;

                add_to_items(-0.5f, -0.5f, -0.5f);        // Vertex 0
                add_to_items(x, y, z);                    // World position 0
                add_to_items(0.0f, 1.0f, 0.0f);           // Normal 0
                add_to_items(r, g, b);                    // Color 0
                add_to_items(tx, ty);                     // Texture 0
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(-0.5f, -0.5f,  0.5f);        // Vertex 1
                add_to_items(x, y, z);                    // World position 1
                add_to_items(0.0f, 1.0f, 0.0f);           // Normal 1
                add_to_items(r, g, b);                    // Color 1
                add_to_items(tx, ty + tsize_y);           // Texture 1
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(0.5f, -0.5f,  0.5f);         // Vertex 2
                add_to_items(x, y, z);                    // World position 2
                add_to_items(0.0f, 1.0f, 0.0f);           // Normal 2
                add_to_items(r, g, b);                    // Color 2
                add_to_items(tx + tsize_x, ty + tsize_y); // Texture 2
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(0.5f, -0.5f, -0.5f);         // Vertex 3
                add_to_items(x, y, z);                    // World position 3
                add_to_items(0.0f, 1.0f, 0.0f);           // Normal 3
                add_to_items(r, g, b);                    // Color 3
                add_to_items(tx + tsize_x, ty);           // Texture 3
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                n_quads += 4;
            }

            void add_left(const float x, const float y, const float z, float r, float g, float b, const float tx,
                          float ty, const int &idx, const bool &above_ground,
                          const float &light_r, const float &light_g, const float &light_b,
                          const float &light_x, const float &light_y, const float &light_z)
            {
                colorize_and_texture_other(idx, r, g, b, ty);
                const float ground_indicator = above_ground ? 255.0f : 0.0f;

                add_to_items(-0.5f, -0.5f, -0.5f);        // Vertex 0
                add_to_items(x, y, z);                    // World position 0
                add_to_items(-1.0f, 0.0f, 0.0f);          // Normal 0
                add_to_items(r, g, b);                    // Color 0
                add_to_items(tx, ty + tsize_y);           // Texture 0
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(-0.5f, 0.5f,  -0.5f);        // Vertex 1
                add_to_items(x, y, z);                    // World position 1
                add_to_items(-1.0f, 0.0f, 0.0f);           // Normal 1
                add_to_items(r, g, b);                    // Color 1
                add_to_items(tx + tsize_x, ty + tsize_y); // Texture 1
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(-0.5f, 0.5f,  0.5f);         // Vertex 2
                add_to_items(x, y, z);                    // World position 2
                add_to_items(-1.0f, 0.0f, 0.0f);          // Normal 2
                add_to_items(r, g, b);                    // Color 2
                add_to_items(tx + tsize_x, ty);           // Texture 2
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(-0.5f, -0.5f, 0.5f);         // Vertex 3
                add_to_items(x, y, z);                    // World position 3
                add_to_items(-1.0f, 0.0f, 0.0f);           // Normal 3
                add_to_items(r, g, b);                    // Color 3
                add_to_items(tx, ty);                     // Texture 3
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                n_quads += 4;
            }

            void add_right(const float x, const float y, const float z, float r, float g, float b, const float tx,
                           float ty, const int &idx, const bool &above_ground,
                           const float &light_r, const float &light_g, const float &light_b,
                           const float &light_x, const float &light_y, const float &light_z)
            {
                colorize_and_texture_other(idx, r, g, b, ty);
                const float ground_indicator = above_ground ? 255.0f : 0.0f;

                add_to_items(0.5f, -0.5f, -0.5f);        // Vertex 0
                add_to_items(x, y, z);                    // World position 0
                add_to_items(1.0f, 0.0f, 0.0f);          // Normal 0
                add_to_items(r, g, b);                    // Color 0
                add_to_items(tx, ty + tsize_y);           // Texture 0
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(0.5f, 0.5f,  -0.5f);        // Vertex 1
                add_to_items(x, y, z);                    // World position 1
                add_to_items(1.0f, 0.0f, 0.0f);           // Normal 1
                add_to_items(r, g, b);                    // Color 1
                add_to_items(tx + tsize_x, ty + tsize_y); // Texture 1
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(0.5f, 0.5f,  0.5f);         // Vertex 2
                add_to_items(x, y, z);                    // World position 2
                add_to_items(1.0f, 0.0f, 0.0f);          // Normal 2
                add_to_items(r, g, b);                    // Color 2
                add_to_items(tx + tsize_x, ty);           // Texture 2
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(0.5f, -0.5f, 0.5f);         // Vertex 3
                add_to_items(x, y, z);                    // World position 3
                add_to_items(1.0f, 0.0f, 0.0f);           // Normal 3
                add_to_items(r, g, b);                    // Color 3
                add_to_items(tx, ty);                     // Texture 3
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                n_quads += 4;
            }

            void add_north(const float x, const float y, const float z, float r, float g, float b, const float tx,
                           float ty, const int &idx, const bool &above_ground,
                           const float &light_r, const float &light_g, const float &light_b,
                           const float &light_x, const float &light_y, const float &light_z)
            {
                colorize_and_texture_other(idx, r, g, b, ty);
                const float ground_indicator = above_ground ? 255.0f : 0.0f;

                add_to_items(-0.5f, -0.5f, -0.5f);        // Vertex 0
                add_to_items(x, y, z);                    // World position 0
                add_to_items(0.0f, 0.0f, -1.0f);          // Normal 0
                add_to_items(r, g, b);                    // Color 0
                add_to_items(tx, ty + tsize_y);           // Texture 0
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(0.5f, -0.5f,  -0.5f);        // Vertex 1
                add_to_items(x, y, z);                    // World position 1
                add_to_items(0.0f, 0.0f, -1.0f);           // Normal 1
                add_to_items(r, g, b);                    // Color 1
                add_to_items(tx + tsize_x, ty + tsize_y); // Texture 1
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(0.5f, 0.5f,  -0.5f);         // Vertex 2
                add_to_items(x, y, z);                    // World position 2
                add_to_items(0.0f, 0.0f, -1.0f);          // Normal 2
                add_to_items(r, g, b);                    // Color 2
                add_to_items(tx + tsize_x, ty);           // Texture 2
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(-0.5f, 0.5f, -0.5f);         // Vertex 3
                add_to_items(x, y, z);                    // World position 3
                add_to_items(0.0f, 0.0f, -1.0f);           // Normal 3
                add_to_items(r, g, b);                    // Color 3
                add_to_items(tx, ty);                     // Texture 3
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                n_quads += 4;
            }

            void add_south(const float x, const float y, const float z, float r, float g, float b, const float tx,
                           float ty, const int &idx, const bool &above_ground,
                           const float &light_r, const float &light_g, const float &light_b,
                           const float &light_x, const float &light_y, const float &light_z)
            {
                colorize_and_texture_other(idx, r, g, b, ty);
                const float ground_indicator = above_ground ? 255.0f : 0.0f;

                add_to_items(-0.5f, -0.5f, 0.5f);        // Vertex 0
                add_to_items(x, y, z);                    // World position 0
                add_to_items(0.0f, 0.0f, 1.0f);          // Normal 0
                add_to_items(r, g, b);                    // Color 0
                add_to_items(tx, ty + tsize_y);           // Texture 0
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(0.5f, -0.5f,  0.5f);        // Vertex 1
                add_to_items(x, y, z);                    // World position 1
                add_to_items(0.0f, 0.0f, 1.0f);           // Normal 1
                add_to_items(r, g, b);                    // Color 1
                add_to_items(tx + tsize_x, ty + tsize_y); // Texture 1
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(0.5f, 0.5f,  0.5f);         // Vertex 2
                add_to_items(x, y, z);                    // World position 2
                add_to_items(0.0f, 0.0f, 1.0f);          // Normal 2
                add_to_items(r, g, b);                    // Color 2
                add_to_items(tx + tsize_x, ty);           // Texture 2
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                add_to_items(-0.5f, 0.5f, 0.5f);         // Vertex 3
                add_to_items(x, y, z);                    // World position 3
                add_to_items(0.0f, 0.0f, 1.0f);           // Normal 3
                add_to_items(r, g, b);                    // Color 3
                add_to_items(tx, ty);                     // Texture 3
                add_to_items(ground_indicator, 0.0f, 0.0f);
                add_to_items(light_r, light_g, light_b, light_x, light_y, light_z);

                n_quads += 4;
            }

            void add_cube(const float x, const float y, const float z, const float r, const float g, const float b, const float tx, const float ty)
            {
                const int idx = mapidx(x, y, z);
                bool above_ground = region::above_ground(idx);
                //if (above_ground) std::cout << "Above ground\n";

                float light_r, light_g, light_b, light_x, light_y, light_z;
                auto light_finder = lit_tiles.find(idx);
                if (light_finder != lit_tiles.end()) {
                    light_r = (float)light_finder->second.second.r / 255.0f;
                    light_g = (float)light_finder->second.second.g / 255.0f;
                    light_b = (float)light_finder->second.second.b / 255.0f;
                    int lx,ly,lz;
                    std::tie(lx,ly,lz) = idxmap(light_finder->second.first);
                    light_x = (float)lx / 255.0f;
                    light_y = (float)lz / 255.0f;
                    light_z = (float)ly / 255.0f;
                    //std::cout << "Light hit at " << light_x << "/" << light_y << "/" << light_z << "\n";
                } else {
                    light_r = 0.0f;
                    light_g = 0.0f;
                    light_b = 0.0f;
                    light_x = 0.0f;
                    light_y = 0.0f;
                    light_z = 0.0f;
                }

                add_floor(x, y, z, r, g, b, tx, ty, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z);
                if (!region::solid(mapidx(x-1, y, z))) add_left(x, y, z, r, g, b, tx, ty, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z);
                if (!region::solid(mapidx(x+1, y, z))) add_right(x, y, z, r, g, b, tx, ty, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z);
                if (!region::solid(mapidx(x, y-1, z))) add_north(x, y, z, r, g, b, tx, ty, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z);
                if (!region::solid(mapidx(x, y+1, z))) add_south(x, y, z, r, g, b, tx, ty, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z);
                add_floor(x, y, z+1.0f, r, g, b, tx, ty, idx, above_ground, light_r, light_g, light_b, light_x, light_y, light_z);
            }
        };
    }

    /*
     * Describes a 16x16x16 chunk of world scenery
     */
    constexpr int n_floats = 23;

    struct chunk_t {
        chunk_t() : index(0), base_x(0), base_y(0), base_z(0) {}
        chunk_t(const int idx, const int x, const int y, const int z) : index(idx), base_x(x),
                                                                        base_y(y), base_z(z)
        {
        }

        int index;
        int base_x, base_y, base_z;
        bool dirty = true;
        bool has_geometry = false;

        bool generated_vbo = false;
        GLuint vbo_id;
        int n_quads = 0;

        std::unique_ptr<geometry::chunk_geometry_t> geometry;
        boost::container::flat_map<int, std::size_t> z_offsets;

        template <typename FUNC>
        void iterate_region(const FUNC &f) {
            for (int z=std::max(base_z, 1); z<std::min(base_z+CHUNK_SIZE, REGION_DEPTH-1); ++z) {
                for (int y=std::max(base_y, 1); y<std::min(base_y+CHUNK_SIZE, REGION_HEIGHT-1); ++y) {
                    for (int x=std::max(base_x, 1); x<std::min(base_x+CHUNK_SIZE, REGION_WIDTH-1); ++x) {
                        f(x, y, z);
                    }
                }
            }
        }
    };

    extern std::vector<chunk_t> chunks;

    void build_chunk_buffer();
    void update_dirty_chunks();
    std::size_t update_chunk(chunk_t &chunk);
}