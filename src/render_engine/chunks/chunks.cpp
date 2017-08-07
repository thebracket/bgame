#include "chunks.hpp"
#include <mutex>
#include "../../bengine/threadpool.h"
#include "../../planet/indices.hpp"
#include "../../planet/region/region.hpp"
#include "../../bengine/gl_include.hpp"
#include <boost/container/flat_set.hpp>

namespace chunks {
    std::array<chunk_t, CHUNKS_TOTAL> chunks;
    bool chunks_initialized = false;

    // Dirty chunks
    std::vector<bool> dirty;
    std::mutex dirty_mutex;

    // Vertex geometry updates; need to not be multi-threaded
    boost::container::flat_set<int> dirty_buffers;
    std::mutex dirty_buffer_mutex;

    void mark_chunk_dirty(const int &idx) {
        std::lock_guard<std::mutex> lock(dirty_mutex);
        dirty[idx] = true;
    }

    void mark_chunk_clean(const int &idx) {
        std::lock_guard<std::mutex> lock(dirty_mutex);
        dirty[idx] = false;
    }

    void enqueue_vertex_update(const int &idx) {
        std::lock_guard<std::mutex> lock(dirty_buffer_mutex);
        dirty_buffers.insert(idx);
    }

    void initialize_chunks() {
        dirty.resize(CHUNKS_TOTAL);
        for (int z=0; z<CHUNK_DEPTH; ++z) {
            for (int y=0; y<CHUNK_HEIGHT; ++y) {
                for (int x=0; x<CHUNK_WIDTH; ++x) {
                    const int idx = chunk_idx(x,y,z);
                    chunks[idx].setup(idx, x*CHUNK_SIZE, y*CHUNK_SIZE, z*CHUNK_SIZE);
                    mark_chunk_dirty(idx);
                }
            }
        }
        chunks_initialized = true;
    }

    void update_chunk(int thread_num, const int &idx) {
        mark_chunk_clean(idx);
        chunks[idx].ready.store(false);
        chunks[idx].update();
    }

    void update_dirty_chunks() {
        std::lock_guard<std::mutex> lock(dirty_mutex);
        for (int i=0; i<CHUNKS_TOTAL; ++i) {
            if (dirty[i]) {
                bengine::thread_pool->push(update_chunk, i);
            }
        }
    }

    unsigned int get_floor_tex(const int &idx) {
        // TODO: Determine the type of floor from a combination of material and vegetation cover
        return 1;
    }

    unsigned int get_cube_tex(const int &idx) {
        // TODO: Determine the type of cube from material
        return 1;
    }

    bool is_cube(const uint8_t type) {
        switch (type) {
            case tile_type::SOLID : return true;
            case tile_type::WALL : return true;
            default: return false;
        }
    }

    void chunk_t::update() {
        dirty[index] = false; // We're guaranteed to already be mutex-protected
        for (int chunk_z = 0; chunk_z < CHUNK_SIZE; ++chunk_z) {
            layer_requires_render.reset(chunk_z);
            has_geometry = false;

            boost::container::flat_map<int, unsigned int> floors;
            boost::container::flat_map<int, unsigned int> cubes;

            const int region_z = chunk_z + base_z;
            for (int chunk_y = 0; chunk_y < CHUNK_SIZE; ++chunk_y) {
                const int region_y = chunk_y + base_y;
                for (int chunk_x = 0; chunk_x < CHUNK_SIZE; ++chunk_x) {
                    const int region_x = chunk_x + base_x;
                    const int idx = mapidx(region_x, region_y, region_z);

                    if (region::revealed(idx)) {
                        const auto tiletype = region::tile_type(idx);
                        if (tiletype == tile_type::FLOOR) {
                            floors[idx] = get_floor_tex(idx);
                        } else if (is_cube(tiletype)) {
                            cubes[idx] = get_cube_tex(idx);
                        }
                    }
                }
            }

            layers[chunk_z].vertices.clear();
            if (!floors.empty()) greedy_floors(floors, chunk_z);
            if (!cubes.empty()) greedy_cubes(cubes, chunk_z);

            if (!floors.empty() || !cubes.empty()) layer_requires_render.set(chunk_z);
            enqueue_vertex_update(index);
        }

        has_geometry = layer_requires_render.count() > 0;
    }

    void chunk_t::greedy_floors(boost::container::flat_map<int, unsigned int> &floors, const int &layer) {
        std::size_t n_floors = floors.size();

        //std::cout << floors.size() << " floors to process\n";

        while (!floors.empty()) {
            const auto first_floor = floors.begin();
            const auto base_region_idx = first_floor->first;
            const auto texture_id = first_floor->second;

            int tile_x, tile_y, tile_z;
            std::tie(tile_x, tile_y, tile_z) = idxmap(base_region_idx);
            int width = 1;
            int height = 1;

            floors.erase(base_region_idx);
            int idx_grow_right = base_region_idx+1;
            int x_coordinate = tile_x;
            auto right_finder = floors.find(idx_grow_right);
            while (x_coordinate < REGION_WIDTH-1 && idx_grow_right < mapidx(std::min(REGION_WIDTH-1, base_x + CHUNK_SIZE), tile_y, tile_z) && right_finder != floors.end() && right_finder->second == texture_id) {
                floors.erase(idx_grow_right);
                ++width;
                ++idx_grow_right;
                ++x_coordinate;
                right_finder = floors.find(idx_grow_right);
            }

            //std::cout << "Merging " << width << " tiles horizontally\n";

            if (tile_y < REGION_HEIGHT-1) {
                int y_progress = tile_y + 1;

                while (y_progress < base_y + CHUNK_SIZE && y_progress < REGION_HEIGHT-1) {
                    bool possible = true;
                    for (int gx = tile_x; gx < tile_x + width; ++gx) {
                        const int candidate_idx = mapidx(gx, y_progress, tile_z);
                        auto vfinder = floors.find(candidate_idx);
                        if (!(vfinder != floors.end() && vfinder->second == texture_id)) possible = false;
                    }
                    if (possible) {
                        ++height;
                        for (int gx = tile_x; gx < tile_x + width; ++gx) {
                            const int candidate_idx = mapidx(gx, y_progress, tile_z);
                            floors.erase(candidate_idx);
                        }
                    }

                    ++y_progress;
                }
            }
            //std::cout << "Merging with " << height << " tiles vertically\n";

            // TODO: Emit geometry
            //std::cout << floors.size() << " floors remain\n";
            layers[layer].vertices.emplace_back(
                    geometry_t{ 0.0f, (float)tile_x, (float)tile_y, (float)tile_z, (float)width, (float)height, (float)texture_id }
            );
        }
        //std::cout << "Reduced " << n_floors << " FLOORS to " << layers[layer].floor_vertices.size() << " items of geometry.\n";
    }

    void chunk_t::greedy_cubes(boost::container::flat_map<int, unsigned int> &cubes, const int &layer) {
        std::size_t n_cubes = cubes.size();

        while (!cubes.empty()) {
            const auto first_floor = cubes.begin();
            const auto base_region_idx = first_floor->first;
            const auto texture_id = first_floor->second;

            int tile_x, tile_y, tile_z;
            std::tie(tile_x, tile_y, tile_z) = idxmap(base_region_idx);
            int width = 1;
            int height = 1;

            cubes.erase(base_region_idx);
            int idx_grow_right = base_region_idx+1;
            int x_coordinate = tile_x;
            auto right_finder = cubes.find(idx_grow_right);
            while (x_coordinate < REGION_WIDTH-1 && idx_grow_right < mapidx(std::min(REGION_WIDTH-1, base_x + CHUNK_SIZE), tile_y, tile_z) && right_finder != cubes.end() && right_finder->second == texture_id) {
                cubes.erase(idx_grow_right);
                ++width;
                ++idx_grow_right;
                ++x_coordinate;
                right_finder = cubes.find(idx_grow_right);
            }

            //std::cout << "Merging " << width << " tiles horizontally\n";

            if (tile_y < REGION_HEIGHT-1) {
                int y_progress = tile_y + 1;

                while (y_progress < base_y + CHUNK_SIZE && y_progress < REGION_HEIGHT-1) {
                    bool possible = true;
                    for (int gx = tile_x; gx < tile_x + width; ++gx) {
                        const int candidate_idx = mapidx(gx, y_progress, tile_z);
                        auto vfinder = cubes.find(candidate_idx);
                        if (!(vfinder != cubes.end() && vfinder->second == texture_id)) possible = false;
                    }
                    if (possible) {
                        ++height;
                        for (int gx = tile_x; gx < tile_x + width; ++gx) {
                            const int candidate_idx = mapidx(gx, y_progress, tile_z);
                            cubes.erase(candidate_idx);
                        }
                    }

                    ++y_progress;
                }
            }
            //std::cout << "Merging with " << height << " tiles vertically\n";

            // TODO: Emit geometry
            //std::cout << floors.size() << " floors remain\n";
            layers[layer].vertices.emplace_back(
                    geometry_t{ 1.0f, (float)tile_x, (float)tile_y, (float)tile_z, (float)width, (float)height, (float)texture_id }
            );
        }
        //std::cout << "Reduced " << n_cubes << " CUBES to " << layers[layer].cube_vertices.size() << " items of geometry.\n";
    }

    void add(std::vector<float> &v, const float &x, const float &y, const float &z, const float &id, const float &nx, const float &ny, const float &nz) {
        v.emplace_back(x);
        v.emplace_back(y);
        v.emplace_back(z);
        v.emplace_back(id);
        v.emplace_back(nx);
        v.emplace_back(ny);
        v.emplace_back(nz);

        //std::cout << x << "/" << y << "/" << z << "\n";
    }

    void chunk_t::update_buffer() {
        for (auto &layer : layers) {
            if (layer.vertices.empty()) {
                //if (layer.vao > 0) glDeleteVertexArrays(1, &layer.vao);
                //if (layer.vbo > 0) glDeleteBuffers(1, &layer.vbo);
            } else {
                if (layer.vao == 0) glGenVertexArrays(1, &layer.vao);
                if (layer.vbo == 0) glGenBuffers(1, &layer.vbo);

                // Bind and map the geometry
                std::vector<float> v;

                // Generate the actual geometry
                for (const auto &item : layer.vertices) {
                    if (item.type == 0.0f) {
                        // It's a floor
                        add(v, -0.5f + item.x, -0.5f + item.z, -0.5f + item.y, item.texture_id, 0.0f, 1.0f, 0.0f);
                        add(v, 0.5f + item.x + item.width, -0.5f + item.z, -0.5f + item.y, item.texture_id, 0.0f, 1.0f, 0.0f);
                        add(v, 0.5f + item.x + item.width, -0.5f + item.z, 0.5f + item.y + item.height, item.texture_id, 0.0f, 1.0f, 0.0f);
                        add(v, 0.5f + item.x + item.width, -0.5f + item.z, 0.5f + item.y + item.height, item.texture_id, 0.0f, 1.0f, 0.0f);
                        add(v, -0.5f + item.x, -0.5f + item.z, 0.5f + item.y + item.height, item.texture_id, 0.0f, 1.0f, 0.0f);
                        add(v, -0.5f + item.x, -0.5f + item.z, -0.5f + item.y, item.texture_id, 0.0f, 1.0f, 0.0f);
                    } else {
                        // It's a cube; add all 6 sides eventually

                        // Floor
                        add(v, -0.5f + item.x, -0.5f + item.z, -0.5f + item.y, item.texture_id, 0.0f, -1.0f, 0.0f);
                        add(v, 0.5f + item.x + item.width, -0.5f + item.z, -0.5f + item.y, item.texture_id, 0.0f, -1.0f, 0.0f);
                        add(v, 0.5f + item.x + item.width, -0.5f + item.z, 0.5f + item.y + item.height, item.texture_id, 0.0f, -1.0f, 0.0f);
                        add(v, 0.5f + item.x + item.width, -0.5f + item.z, 0.5f + item.y + item.height, item.texture_id, 0.0f, -1.0f, 0.0f);
                        add(v, -0.5f + item.x, -0.5f + item.z, 0.5f + item.y + item.height, item.texture_id, 0.0f, -1.0f, 0.0f);
                        add(v, -0.5f + item.x, -0.5f + item.z, -0.5f + item.y, item.texture_id, 0.0f, -1.0f, 0.0f);
                    }
                }
                layer.vertices.clear();

                // Bind and map
                glBindVertexArray(layer.vao);
                glBindBuffer(GL_ARRAY_BUFFER, layer.vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size(), &v[0], GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, layer.vbo);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0); // 0 = Vertex Position

                glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (char *) nullptr + 3 * sizeof(float));
                glEnableVertexAttribArray(1); // 1 = Texture ID

                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (char *) nullptr + 4 * sizeof(float));
                glEnableVertexAttribArray(2); // 2 = Normals

                glBindVertexArray(0);

                //std::cout << "Bound " << v.size() << " elements to VBO " << layer.vao << "/" << layer.vbo << "\n";
                layer.n_elements = v.size();
                has_geometry = true;
            }
        }
    }

    void update_buffers() {
        std::lock_guard<std::mutex> lock(dirty_buffer_mutex);
        for (const auto &idx : dirty_buffers) {
            std::cout << "Updating buffer " << idx << "\n";
            chunks[idx].update_buffer();
            chunks[idx].ready.store(true);
        }
        dirty_buffers.clear();
    }

}