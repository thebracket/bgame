#include "chunk.hpp"
#include "../../planet/region/region.hpp"
#include <algorithm>
#include <vector>

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
        std::size_t n_updates = 0;
        std::for_each(chunks.begin(), chunks.end(), [&n_updates] (chunk_t &chunk) {
            if (chunk.dirty) {
                n_updates += update_chunk(chunk);
            }
        });
        //std::cout << "Done with chunk updates\n";
        /*if (n_updates >0) {
            std::cout << "Emitted " << n_updates << " quads.\n";
        }*/
    }

    bool render_as_cube(const uint8_t &type) {
        switch (type) {
            case tile_type::SOLID   : return true;
            case tile_type::SEMI_MOLTEN_ROCK : return true;
            case tile_type::WALL : return true;
            case tile_type::TREE_TRUNK : return true;
            default                 : return false;
        }
    }

    std::size_t update_chunk(chunk_t &chunk) {
        // We want to make a big render buffer for the terrain in this block.
        chunk.has_geometry = false;
        if (chunk.geometry) chunk.geometry.reset();
        chunk.geometry = std::make_unique<geometry::chunk_geometry_t>();
        chunk.n_quads = 0;

        int last_z = chunk.base_z;
        chunk.iterate_region([&chunk, &last_z] (int x,int y, int z) {
            if (last_z != z) {
                chunk.z_offsets[last_z] = chunk.geometry->n_quads;
                last_z = z;
                //std::cout << "Z offset stored: " << last_z << ", " << chunk.geometry->items.size() << "\n";
            }

            //printf("Iterating %d/%d/%d as part of %d\n", x, y, z, chunk.index);
            const int idx = mapidx(x,y,z);
            const auto tiletype = region::tile_type(idx);

            const auto render = region::render_cache(idx);
            const float r = 1.0f;
            const float g = 1.0f;
            const float b = 1.0f;
            constexpr float tx = 0.0f;
            constexpr float ty = 0.0f;

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
                    chunk.geometry->add_cube(x,y,z,r,g,b,tx,ty);
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
                        light_x = (float)lx;
                        light_y = (float)lz;
                        light_z = (float)ly;
                    } else {
                        light_r = 0.0f;
                        light_g = 0.0f;
                        light_b = 0.0f;
                        light_x = 0.0f;
                        light_y = 0.0f;
                        light_z = 0.0f;
                    }
                    chunk.geometry->add_floor(x,y,z,r,g,b,tx,ty,idx,region::above_ground(idx), light_r, light_g, light_b, light_x, light_y, light_z);
                }
            }
        });
        chunk.dirty = false;

        chunk.n_quads = chunk.geometry->n_quads;

        if (chunk.generated_vbo) {
            //std::cout << "Buffer deletion\n";
            glDeleteBuffers(1, &chunk.vbo_id);
        }

        if (chunk.has_geometry || chunk.geometry->items.size()>0) {
            //std::cout << "Generating a buffer of " << (chunk.geometry->items.size() / n_floats) << " quads.\n";
            glGenBuffers(1, &chunk.vbo_id); // Generate the VBO
            //std::cout << "Buffer ID: " << chunk.vbo_id << "\n";
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.vbo_id);

            if (!chunk.geometry) {
                throw std::runtime_error("Geometry chunk does not exist");
            }
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * chunk.geometry->items.size(),
                         &chunk.geometry->items[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            chunk.generated_vbo = true;

            return chunk.geometry->items.size() / n_floats;
        }
        return 0;
    }
}