#pragma once
#include "../../planet/constants.hpp"
#include "render_block.hpp"
#include <array>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif

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
        constexpr float texture_width = 384.0f;
        constexpr float texture_height = 768.0f;
        constexpr float sprite_width = 24.0f;
        constexpr float sprite_height = 24.0f;
        constexpr float tsize_x = (1.0f / texture_width) * sprite_width;
        constexpr float tsize_y = (1.0f / texture_height) * sprite_height;

        struct chunk_geometry_t {
            /*
             * Layout:
             * 0,1,2 = Vertex Position
             * 3,4,5 = World Position
             * 6,7,8 = Normal
             * 9,10,11 = Color
             * 12,13 = Texture Position
             */
            std::vector<float> items;
            int n_quads = 0;

            template<typename T> inline T add_to_items_impl(T arg) { items.emplace_back(arg); return arg; }

            template<typename... T>
            inline void add_to_items(T... args) {
                float float_array[] = { add_to_items_impl(args)... };
            }

            void add_floor(const float x, const float y, const float z, const float r, const float g, const float b, const float tx, const float ty) {
                add_to_items(-0.5f, -0.5f, -0.5f);        // Vertex 0
                add_to_items(x, y, z);                    // World position 0
                add_to_items(0.0f, 1.0f, 0.0f);           // Normal 0
                add_to_items(r, g, b);                    // Color 0
                add_to_items(tx, ty);                     // Texture 0

                add_to_items(-0.5f, -0.5f,  0.5f);        // Vertex 1
                add_to_items(x, y, z);                    // World position 1
                add_to_items(0.0f, 1.0f, 0.0f);           // Normal 1
                add_to_items(r, g, b);                    // Color 1
                add_to_items(tx, ty + tsize_y);           // Texture 1

                add_to_items(0.5f, -0.5f,  0.5f);         // Vertex 2
                add_to_items(x, y, z);                    // World position 2
                add_to_items(0.0f, 1.0f, 0.0f);           // Normal 2
                add_to_items(r, g, b);                    // Color 2
                add_to_items(tx + tsize_x, ty + tsize_y); // Texture 2

                add_to_items(0.5f, -0.5f, -0.5f);         // Vertex 3
                add_to_items(x, y, z);                    // World position 3
                add_to_items(0.0f, 1.0f, 0.0f);           // Normal 3
                add_to_items(r, g, b);                    // Color 3
                add_to_items(tx + tsize_x, ty);           // Texture 3

                n_quads += 4;
            }

            void add_left(const float x, const float y, const float z, const float r, const float g, const float b, const float tx, const float ty) {
                add_to_items(-0.5f, -0.5f, -0.5f);        // Vertex 0
                add_to_items(x, y, z);                    // World position 0
                add_to_items(-1.0f, 0.0f, 0.0f);          // Normal 0
                add_to_items(r, g, b);                    // Color 0
                add_to_items(tx, ty + tsize_y);           // Texture 0

                add_to_items(-0.5f, 0.5f,  -0.5f);        // Vertex 1
                add_to_items(x, y, z);                    // World position 1
                add_to_items(-1.0f, 0.0f, 0.0f);           // Normal 1
                add_to_items(r, g, b);                    // Color 1
                add_to_items(tx + tsize_x, ty + tsize_y); // Texture 1

                add_to_items(-0.5f, 0.5f,  0.5f);         // Vertex 2
                add_to_items(x, y, z);                    // World position 2
                add_to_items(-1.0f, 0.0f, 0.0f);          // Normal 2
                add_to_items(r, g, b);                    // Color 2
                add_to_items(tx + tsize_x, ty);           // Texture 2

                add_to_items(-0.5f, -0.5f, 0.5f);         // Vertex 3
                add_to_items(x, y, z);                    // World position 3
                add_to_items(-1.0f, 0.0f, 0.0f);           // Normal 3
                add_to_items(r, g, b);                    // Color 3
                add_to_items(tx, ty);                     // Texture 3

                n_quads += 4;
            }

            void add_right(const float x, const float y, const float z, const float r, const float g, const float b, const float tx, const float ty) {
                add_to_items(0.5f, -0.5f, -0.5f);        // Vertex 0
                add_to_items(x, y, z);                    // World position 0
                add_to_items(1.0f, 0.0f, 0.0f);          // Normal 0
                add_to_items(r, g, b);                    // Color 0
                add_to_items(tx, ty + tsize_y);           // Texture 0

                add_to_items(0.5f, 0.5f,  -0.5f);        // Vertex 1
                add_to_items(x, y, z);                    // World position 1
                add_to_items(1.0f, 0.0f, 0.0f);           // Normal 1
                add_to_items(r, g, b);                    // Color 1
                add_to_items(tx + tsize_x, ty + tsize_y); // Texture 1

                add_to_items(0.5f, 0.5f,  0.5f);         // Vertex 2
                add_to_items(x, y, z);                    // World position 2
                add_to_items(1.0f, 0.0f, 0.0f);          // Normal 2
                add_to_items(r, g, b);                    // Color 2
                add_to_items(tx + tsize_x, ty);           // Texture 2

                add_to_items(0.5f, -0.5f, 0.5f);         // Vertex 3
                add_to_items(x, y, z);                    // World position 3
                add_to_items(1.0f, 0.0f, 0.0f);           // Normal 3
                add_to_items(r, g, b);                    // Color 3
                add_to_items(tx, ty);                     // Texture 3

                n_quads += 4;
            }

            void add_north(const float x, const float y, const float z, const float r, const float g, const float b, const float tx, const float ty) {
                add_to_items(-0.5f, -0.5f, -0.5f);        // Vertex 0
                add_to_items(x, y, z);                    // World position 0
                add_to_items(0.0f, -1.0f, 0.0f);          // Normal 0
                add_to_items(r, g, b);                    // Color 0
                add_to_items(tx, ty + tsize_y);           // Texture 0

                add_to_items(0.5f, -0.5f,  -0.5f);        // Vertex 1
                add_to_items(x, y, z);                    // World position 1
                add_to_items(0.0f, -1.0f, 0.0f);           // Normal 1
                add_to_items(r, g, b);                    // Color 1
                add_to_items(tx + tsize_x, ty + tsize_y); // Texture 1

                add_to_items(0.5f, -0.5f,  0.5f);         // Vertex 2
                add_to_items(x, y, z);                    // World position 2
                add_to_items(0.0f, -1.0f, 0.0f);          // Normal 2
                add_to_items(r, g, b);                    // Color 2
                add_to_items(tx + tsize_x, ty);           // Texture 2

                add_to_items(-0.5f, -0.5f, 0.5f);         // Vertex 3
                add_to_items(x, y, z);                    // World position 3
                add_to_items(0.0f, -1.0f, 0.0f);           // Normal 3
                add_to_items(r, g, b);                    // Color 3
                add_to_items(tx, ty);                     // Texture 3

                n_quads += 4;
            }

            void add_south(const float x, const float y, const float z, const float r, const float g, const float b, const float tx, const float ty) {
                add_to_items(-0.5f, 0.5f, -0.5f);        // Vertex 0
                add_to_items(x, y, z);                    // World position 0
                add_to_items(0.0f, 1.0f, 0.0f);          // Normal 0
                add_to_items(r, g, b);                    // Color 0
                add_to_items(tx, ty + tsize_y);           // Texture 0

                add_to_items(0.5f, 0.5f,  -0.5f);        // Vertex 1
                add_to_items(x, y, z);                    // World position 1
                add_to_items(0.0f, 1.0f, 0.0f);           // Normal 1
                add_to_items(r, g, b);                    // Color 1
                add_to_items(tx + tsize_x, ty + tsize_y); // Texture 1

                add_to_items(0.5f, 0.5f,  0.5f);         // Vertex 2
                add_to_items(x, y, z);                    // World position 2
                add_to_items(0.0f, 1.0f, 0.0f);          // Normal 2
                add_to_items(r, g, b);                    // Color 2
                add_to_items(tx + tsize_x, ty);           // Texture 2

                add_to_items(-0.5f, 0.5f, 0.5f);         // Vertex 3
                add_to_items(x, y, z);                    // World position 3
                add_to_items(0.0f, 1.0f, 0.0f);           // Normal 3
                add_to_items(r, g, b);                    // Color 3
                add_to_items(tx, ty);                     // Texture 3

                n_quads += 4;
            }

            void add_cube(const float x, const float y, const float z, const float r, const float g, const float b, const float tx, const float ty) {
                add_floor(x, y, z, r, g, b, tx, ty);
                add_left(x, y, z, r, g, b, tx, ty);
                add_right(x, y, z, r, g, b, tx, ty);
                add_north(x, y, z, r, g, b, tx, ty);
                add_south(x, y, z, r, g, b, tx, ty);
                add_floor(x, y+1.0f, z, r, g, b, tx, ty);
            }
        };
    }

    /*
     * Describes a 16x16x16 chunk of world scenery
     */
    constexpr int n_floats = 14;

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