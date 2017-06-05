#include "chunk.hpp"
#include "../../planet/region/region.hpp"
#include <algorithm>
#include <vector>

namespace gl {

    std::array<chunk_t, CHUNKS_TOTAL> chunks;

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

    void update_chunk(chunk_t &chunk) {
        // We want to make a big render buffer for the terrain in this block.
        chunk.has_geometry = false;
        if (chunk.geometry) chunk.geometry.reset();
        chunk.geometry = std::make_unique<geometry::chunk_geometry_t>();

        chunk.iterate_region([&chunk] (int x,int y, int z) {
            //printf("Iterating %d/%d/%d as part of %d\n", x, y, z, chunk.index);
            const int idx = mapidx(x,y,z);
            const auto tiletype = region::tile_type(idx);
            if (tiletype != tile_type::OPEN_SPACE) {
                if (tiletype == tile_type::SOLID) {
                    // TODO: Add cube
                } else if (tiletype == tile_type::FLOOR) {
                    // Add a floor
                    chunk.has_geometry = true;
                    const auto render = region::render_cache(idx);
                    const float r = (float)render.foreground.r / 255.0f;
                    const float g = (float)render.foreground.g / 255.0f;
                    const float b = (float)render.foreground.b / 255.0f;
                    const float tx = (float)(render.glyph % 16) * geometry::tsize_x;
                    const float ty = (float)(render.glyph / 16) * geometry::tsize_y;
                    chunk.geometry->add_floor(x,y,z,r,g,b,tx,ty);
                }
            }
        });
        chunk.dirty = false;

        chunk.n_quads = chunk.geometry->n_quads;

        if (chunk.generated_vbo) {
            glDeleteBuffers(1, &chunk.vbo_id);
        }

        glGenBuffers(1, &chunk.vbo_id); // Generate the VBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.vbo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * n_floats * chunk.geometry->items.size(),
                     &chunk.geometry->items[0], GL_STATIC_DRAW);

        chunk.generated_vbo = true;
    }
}