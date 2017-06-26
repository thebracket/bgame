#pragma once

#include <boost/container/flat_map.hpp>
#include <vector>

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
     */
    constexpr int n_floats = 23;

    /*
     * A geometry bucket contains geometry for a single texture
     */
    struct terrain_bucket_t {

        std::vector<float> items;
        int n_quads = 0;
        bool generated_vbo = false;
        unsigned int vbo_id;
        boost::container::flat_map<int, std::size_t> z_offsets;

        template<typename T> inline T add_to_items_impl(T arg) { items.emplace_back(arg); return arg; }

        template<typename... T>
        inline void add_to_items(T... args) {
            float float_array[] = { add_to_items_impl(args)... };
        }

        void add_floor(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess);

        void add_left(const float x, const float y, const float z, float r, float g, float b,
                      const int &idx, const bool &above_ground,
                      const float &light_r, const float &light_g, const float &light_b,
                      const float &light_x, const float &light_y, const float &light_z, const float &shininess);

        void add_right(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess);

        void add_north(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess);

        void add_south(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess);

        void make_vbo();
    };


    /*
     * A geometry buffer contains buckets divided by texture binding, which in turn contain the actual geometry.
     */
    struct geometry_buffer_t {
        boost::container::flat_map<int, terrain_bucket_t> buckets;

        void add_floor(const float x, const float y, const float z,
                       const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z);

        void add_cube(const float x, const float y, const float z);

        void mark_z_level_end(const int &z);
        void make_vbos();
    };
}