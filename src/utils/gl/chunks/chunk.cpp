#include "chunk.hpp"
#include "../../planet/region/region.hpp"
#include <algorithm>
#include <vector>
#include "geometry_buffer.hpp"

namespace gl {

    std::vector<chunk_t> chunks(CHUNKS_TOTAL);

    void build_chunk_buffer() {
        // Build empty chunks for each slot
        for (int z=0; z<CHUNKS_DEPTH; ++z) {
            for (int y=0; y<CHUNKS_HEIGHT; ++y) {
                for (int x=0; x<CHUNKS_WIDTH; ++x) {
                    const int chunkidx = chunk_idx(x,y,z);
                    chunks[chunkidx] = chunk_t{chunkidx, x*CHUNK_SIZE, y*CHUNK_SIZE, z*CHUNK_SIZE};
                }
            }
        }
    }

    void update_dirty_chunks() {
        std::for_each(chunks.begin(), chunks.end(), [] (chunk_t &chunk) {
            if (chunk.dirty) {
                update_chunk(chunk);
            }
        });
    }

    bool render_as_cube(const uint8_t &type) {
        switch (type) {
            case tile_type::SOLID   : return true;
            case tile_type::SEMI_MOLTEN_ROCK : return true;
            case tile_type::WALL : return true;
            case tile_type::TREE_TRUNK : return true;
            case tile_type::TREE_LEAF : return true;
            default                 : return false;
        }
    }

    void update_chunk(chunk_t &chunk) {
        // We want to make a big render buffer for the terrain in this block.
        chunk.has_geometry = false;
        if (chunk.geometry) chunk.geometry.reset();
        chunk.geometry = std::make_unique<geometry_buffer_t>();

        int last_z = chunk.base_z;
        chunk.iterate_region([&chunk, &last_z] (int x,int y, int z) {
            if (last_z != z) {
                chunk.geometry->mark_z_level_end(last_z);
                last_z = z;
            }

            const int idx = mapidx(x,y,z);
            const auto tiletype = region::tile_type(idx);

            const auto render = region::render_cache(idx);
            const float r = 1.0f;
            const float g = 1.0f;
            const float b = 1.0f;

            bool skip = false;
            // Open space never needs rendering
            if (tiletype == tile_type::OPEN_SPACE) skip = true;

            // If it isn't seen, and isn't an outer edge, we can skip it (we keep outer edges to avoid shining through lights)
            if (!skip && !region::revealed(idx))
            {
                skip = true;
            }

            // TODO: cull tiles with neighbors that completely occlude it.

            if (!skip) {
                if (render_as_cube(tiletype))
                {
                    // Add cube
                    chunk.has_geometry = true;
                    chunk.geometry->add_cube(x,y,z,r,g,b);
                } else if (tiletype == tile_type::FLOOR) {
                    // Add a floor
                    chunk.has_geometry = true;
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
                    } else {
                        light_r = 0.0f;
                        light_g = 0.0f;
                        light_b = 0.0f;
                        light_x = 0.0f;
                        light_y = 0.0f;
                        light_z = 0.0f;
                    }
                    chunk.geometry->add_floor(x,y,z,r,g,b,region::above_ground(idx), light_r, light_g, light_b, light_x, light_y, light_z);
                }
            }
        });
        chunk.dirty = false;

        chunk.geometry->make_vbos();
    }
}