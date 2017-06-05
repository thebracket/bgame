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
        if (n_updates >0) {
            std::cout << "Emitted " << n_updates << " quads.\n";
        }
    }

    std::size_t update_chunk(chunk_t &chunk) {
        // We want to make a big render buffer for the terrain in this block.
        chunk.has_geometry = false;
        if (chunk.geometry) chunk.geometry.reset();
        chunk.geometry = std::make_unique<geometry::chunk_geometry_t>();

        chunk.iterate_region([&chunk] (int x,int y, int z) {
            //printf("Iterating %d/%d/%d as part of %d\n", x, y, z, chunk.index);
            const int idx = mapidx(x,y,z);
            const auto tiletype = region::tile_type(idx);

            const auto render = region::render_cache(idx);
            const float r = (float)render.foreground.r / 255.0f;
            const float g = (float)render.foreground.g / 255.0f;
            const float b = (float)render.foreground.b / 255.0f;
            const float tx = (float)(render.glyph % 16) * geometry::tsize_x;
            const float ty = (float)(render.glyph / 16) * geometry::tsize_y;

            bool skip = false;
            if (tiletype == tile_type::OPEN_SPACE) skip = true;
            if (!skip && !region::revealed(idx)) skip = true;

            if (!skip) {
                if (tiletype == tile_type::SOLID || tiletype == tile_type::WALL  || tiletype == tile_type::TREE_TRUNK
                    || tiletype == tile_type::TREE_LEAF || tiletype == tile_type::SEMI_MOLTEN_ROCK)
                {
                    // Add cube
                    chunk.has_geometry = true;
                    chunk.geometry->add_cube(x,y,z,r,g,b,tx,ty);
                } else {
                    // Add a floor
                    chunk.has_geometry = true;
                    chunk.geometry->add_floor(x,y,z,r,g,b,tx,ty);
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