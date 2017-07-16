#pragma once

#include <vector>
#include <array>
#include "constants.hpp"
#include "terrain_bucket.hpp"

namespace gl {
    /*
     * A geometry buffer contains buckets divided by texture binding, which in turn contain the actual geometry.
     */
    struct geometry_buffer_t {
        terrain_bucket_t bucket;
        int base_z = 0;

        void add_floor(const float x, const float y, const float z, const bool &above_ground);
        void add_cube(const float x, const float y, const float z);
        void add_ramp(const float x, const float y, const float z);

        void mark_z_level_end(const int &z);
        void finish_z_map(const int &base_z);
        void make_vbos();
    };
}