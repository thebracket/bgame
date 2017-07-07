#pragma once

#include <boost/container/flat_map.hpp>
#include <vector>
#include <array>
#include "constants.hpp"

namespace gl {
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
     * 23,24 = Normal Position
     */
    constexpr int n_floats = 25;

    /*
     * A geometry bucket contains geometry for a single texture
     */
    struct terrain_bucket_t {

        terrain_bucket_t() {
            items.reserve(50000);
        }

        std::vector<float> items;
        int n_quads = 0;
        bool generated_vbo = false;
        unsigned int vbo_id;
        std::array<std::size_t, CHUNK_SIZE> z_offsets{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        template<typename T> inline T add_to_items_impl(T arg) { items.emplace_back(arg); return arg; }

        template<typename... T>
        inline void add_to_items(T... args) {
            float float_array[] = { add_to_items_impl(args)... };
        }

        void add_floor(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                       const int &texture_id, const int &normal_id);

        void add_veg(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                       const float &wang, const int &texture_id, const int &normal_id);

        void add_water(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                       const int &texture_id, const int &normal_id, const uint8_t &water_level);

        void add_renderable(const float x, const float y, const float z, float r, float g, float b,
                     const int &idx, const bool &above_ground,
                     const float &light_r, const float &light_g, const float &light_b,
                     const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                     const float &tex_x, const float &tex_y);

        void add_billboard(const float x, const float y, const float z, float r, float g, float b,
                            const int &idx, const bool &above_ground,
                            const float &light_r, const float &light_g, const float &light_b,
                            const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                            const float &tex_x, const float &tex_y);

        void add_left(const float x, const float y, const float z, float r, float g, float b,
                      const int &idx, const bool &above_ground,
                      const float &light_r, const float &light_g, const float &light_b,
                      const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                      const int &texture_id, const int &normal_id);

        void add_right(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                       const int &texture_id, const int &normal_id);

        void add_north(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                       const int &texture_id, const int &normal_id);

        void add_south(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                       const int &texture_id, const int &normal_id);

        void add_slope(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                       const int &texture_id, const int &normal_id, const float nwy, const float ney, const float swy, const float sey);

        void make_vbo();
    };


    /*
     * A geometry buffer contains buckets divided by texture binding, which in turn contain the actual geometry.
     */
    struct geometry_buffer_t {
        terrain_bucket_t bucket;
        int base_z = 0;

        void add_floor(const float x, const float y, const float z,
                       const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z);

        void add_water(const float x, const float y, const float z,
                       const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const uint8_t water_level);

        void add_veg(const float x, const float y, const float z,
                       const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const int &wang,
                       const int texid, const int normid);

        void add_cube(const float x, const float y, const float z);
        void add_ramp(const float x, const float y, const float z);

        void mark_z_level_end(const int &z);
        void finish_z_map(const int &base_z);
        void make_vbos();
    };
}