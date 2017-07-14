#pragma once

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
     */
    constexpr int n_floats = 23;

    /*
     * A geometry bucket contains geometry for a single texture
     */
    struct terrain_bucket_t {

        terrain_bucket_t() {
            items.reserve(75000);
        }

        std::vector<float> items;
        int n_quads = 0;
        bool generated_vbo = false;
        unsigned int vbo_id = 0;
        unsigned int vao_id = 0;
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
                       const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id);

        void add_veg(const float x, const float y, const float z, float r, float g, float b,
                     const int &idx, const bool &above_ground,
                     const float &light_r, const float &light_g, const float &light_b,
                     const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                     const float &wang, const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id);

        void add_water(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                       const int &texture_id, const int &normal_id, const uint8_t &water_level, const int &specular_id, const int &displacement_id);

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
                      const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id);

        void add_right(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                       const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id);

        void add_north(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                       const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id);

        void add_south(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                       const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id);

        void add_slope(const float x, const float y, const float z, float r, float g, float b,
                       const int &idx, const bool &above_ground,
                       const float &light_r, const float &light_g, const float &light_b,
                       const float &light_x, const float &light_y, const float &light_z, const float &shininess,
                       const int &texture_id, const int &normal_id, const int &specular_id, const int &displacement_id,
                       const float nwy, const float ney, const float swy, const float sey);

        void make_vbo();
    };
}