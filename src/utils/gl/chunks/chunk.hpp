#pragma once

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif
#include "../../planet/constants.hpp"
#include "../../planet/indices.hpp"
#include "../../planet/region/region.hpp"
#include "../../raws/materials.hpp"
#include <array>

#include <boost/container/flat_map.hpp>
#include "../../raws/defs/material_def_t.hpp"
#include "../../systems/render/lighting_system.hpp"
#include "constants.hpp"
#include "geometry_buffer.hpp"

namespace gl {
    /*
     * Describes a 16x16x16 chunk of world scenery
     */

    struct chunk_t {
        chunk_t() : index(0), base_x(0), base_y(0), base_z(0) {}

        chunk_t(const int idx, const int x, const int y, const int z) : index(idx), base_x(x),
                                                                        base_y(y), base_z(z) {
        }

        int index;
        int base_x, base_y, base_z;
        bool dirty = true;
        bool has_geometry = false;
        bool veg_dirty = true;
        bool has_vegetation = false;

        std::unique_ptr<geometry_buffer_t> geometry;
        std::unique_ptr<geometry_buffer_t> vegetation;

        template<typename FUNC>
        void iterate_region(const FUNC &f) {
            for (int z = std::max(base_z, 1); z < std::min(base_z + CHUNK_SIZE, REGION_DEPTH - 1); ++z) {
                for (int y = std::max(base_y, 1); y < std::min(base_y + CHUNK_SIZE, REGION_HEIGHT - 1); ++y) {
                    for (int x = std::max(base_x, 1); x < std::min(base_x + CHUNK_SIZE, REGION_WIDTH - 1); ++x) {
                        f(x, y, z);
                    }
                }
            }
        }
    };

    extern std::vector<chunk_t> chunks;

    void build_chunk_buffer();
    void update_dirty_chunks();
    void update_chunk(chunk_t &chunk);
    void update_chunk_vegetation(chunk_t &chunk);
};
