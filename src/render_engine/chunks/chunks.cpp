#include "chunks.hpp"
#include <mutex>
#include "../../bengine/threadpool.h"
#include "../../planet/indices.hpp"
#include "../../planet/region/region.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include <boost/container/flat_set.hpp>
#include <set>
#include <unordered_map>

namespace chunks {
    std::array<chunk_t, CHUNKS_TOTAL> chunks;
    bool chunks_initialized = false;

    // Dirty chunks
    std::vector<bool> dirty;
    std::mutex dirty_mutex;

    // Vertex geometry updates; need to not be multi-threaded
    std::set<int, std::greater<int>> dirty_buffers;
    std::mutex dirty_buffer_mutex;

	void mark_chunk_dirty_by_tileidx(const int &idx) {
		int x, y, z;
		std::tie(x, y, z) = idxmap(idx);
		mark_chunk_dirty(chunk_idx(x / CHUNK_SIZE, y / CHUNK_SIZE, z / CHUNK_SIZE));
	}

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
		mark_chunk_clean(idx);
		std::cout << "Enqueued buffer update\n";
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
        chunks[idx].ready.store(false);
        chunks[idx].update();
    }

    void update_dirty_chunks() {
        std::lock_guard<std::mutex> lock(dirty_mutex);
        for (int i=0; i<CHUNKS_TOTAL; ++i) {
            if (dirty[i]) {
                dirty[i] = false;
                chunks[i].ready.store(false);
                bengine::thread_pool->push(update_chunk, i);
            }
        }
    }

    unsigned int get_floor_tex(const int &idx) {
        if (region::veg_type(idx) > 0) return 0; // Grass is determined to be index 0
        auto material_idx = region::material(idx);
        auto material = get_material(material_idx);
        if (!material) return 3;

        unsigned int use_id = 3;
        if (region::flag(idx, CONSTRUCTION)) {
            use_id = (unsigned int)material->constructed_texture_id;
        } else {
            use_id = (unsigned int)material->base_texture_id;
        }
        if (use_id == 3) std::cout << "Material [" << material->name << "] is lacking a texture\n";

        //std::cout << material->name << "=" << use_id << "\n";
        return use_id;
    }

    unsigned int get_cube_tex(const int &idx) {
        auto tt = region::tile_type(idx);
        if (tt == tile_type::TREE_TRUNK) return 6;
        if (tt == tile_type::TREE_LEAF) return 9;

        auto material_idx = region::material(idx);
        auto material = get_material(material_idx);
        if (!material) return 3;

        unsigned int use_id = 3;
        if (region::flag(idx, CONSTRUCTION)) {
            use_id = (unsigned int)material->constructed_texture_id;
        } else {
            use_id = (unsigned int)material->base_texture_id;
        }
        if (use_id == 3) std::cout << "Material [" << material->name << "] is lacking a texture\n";
        return use_id;
    }

    bool is_cube(const uint8_t type) {
        switch (type) {
            case tile_type::SOLID : return true;
            case tile_type::WALL : return true;
            case tile_type::TREE_LEAF: return true;
            case tile_type::TREE_TRUNK: return true;
            case tile_type::SEMI_MOLTEN_ROCK: return true;
            case tile_type::CLOSED_DOOR: return true;
            default: return false;
        }
    }

    void chunk_t::update() {
        //dirty[index] = false; // We're guaranteed to already be mutex-protected

        for (auto &layer : layers) {
            layer.v.clear();
            layer.n_elements = 0;
			layer.trans.clear();
			layer.n_trans = 0;
        }

        for (int chunk_z = 0; chunk_z < CHUNK_SIZE; ++chunk_z) {
            layer_requires_render.reset(chunk_z);
            has_geometry = false;
			has_transparency = false;

            boost::container::flat_map<int, unsigned int> floors;
            boost::container::flat_map<int, unsigned int> cubes;

            const int region_z = chunk_z + base_z;
            for (int chunk_y = 0; chunk_y < CHUNK_SIZE; ++chunk_y) {
                const int region_y = chunk_y + base_y;
                for (int chunk_x = 0; chunk_x < CHUNK_SIZE; ++chunk_x) {
                    const int region_x = chunk_x + base_x;
                    const int idx = mapidx(region_x, region_y, region_z);

                    const auto tiletype = region::tile_type(idx);
                    if (tiletype != tile_type::OPEN_SPACE) {
                        if (region::revealed(idx)) {
							if (tiletype == tile_type::WINDOW) {
								// TODO: Windows go into transparency buffer
								add_cube_geometry(layers[chunk_z].trans, static_cast<float>(region_x), static_cast<float>(region_y), static_cast<float>(region_z), 1.0f, 1.0f, 15);
								layers[chunk_z].n_trans += 36;
								layer_requires_transparency.set(chunk_z);
							} else if (tiletype == tile_type::FLOOR) {
                                floors[idx] = get_floor_tex(idx);
                            } else if (is_cube(tiletype)) {
                                cubes[idx] = get_cube_tex(idx);
							} else if (tiletype == tile_type::RAMP) {
								// TODO - write me!
								float ne = 0.0f, se = 0.0f, sw = 0.0f, nw = 0.0f;
								if (region::solid(idx - 1)) { sw = 1.0f; nw = 1.0f; }
								else if (region::solid(idx + 1)) { se = 1.0f; ne = 1.0f; }
								else if (region::solid(idx + REGION_WIDTH)) { nw = 1.0f; ne = 1.0f; }
								else if (region::solid(idx - REGION_WIDTH)) { sw = 1.0f; se = 1.0f; }
								add_ramp_geometry(layers[chunk_z].v, region_x, region_y, region_z, 1.0f, 1.0f, get_floor_tex(idx), ne, se, sw, nw);
								layers[chunk_z].n_elements += 6;
							}
                        }
                    }
					if (region::water_level(idx) > 0) {
						add_water_geometry(layers[chunk_z].trans, static_cast<float>(region_x), static_cast<float>(region_y), static_cast<float>(region_z), 1.0f, 1.0f, 12, region::water_level(idx));
						layers[chunk_z].n_trans += 6;
						layer_requires_transparency.set(chunk_z);
					}
                }
            }

            if (!floors.empty()) greedy_floors(floors, chunk_z);
            if (!cubes.empty()) greedy_cubes(cubes, chunk_z);

            if (!floors.empty() || !cubes.empty()) layer_requires_render.set(chunk_z);
        }

        has_geometry = layer_requires_render.count() > 0;
		has_transparency = layer_requires_transparency.count() > 0;

        // Tell GL to update
        enqueue_vertex_update(index);
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

                bool possible = true;
                while (possible && y_progress < base_y + CHUNK_SIZE && y_progress < REGION_HEIGHT-1) {
                    for (int gx = tile_x; gx < tile_x + width; ++gx) {
                        const int candidate_idx = mapidx(gx, y_progress, tile_z);
                        auto vfinder = floors.find(candidate_idx);
                        if (vfinder == floors.end() || vfinder->second != texture_id) possible = false;
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
            add_floor_geometry(layers[layer].v, (float)tile_x, (float)tile_y, (float)tile_z, (float)width, (float)height, (float)texture_id);
            layers[layer].n_elements += 6;
            //layers[layer].vertices.emplace_back(
            //        geometry_t{ 0.0f, (float)tile_x, (float)tile_y, (float)tile_z, (float)width, (float)height, (float)texture_id }
            //);
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

                bool possible = true;
                while (possible && y_progress < base_y + CHUNK_SIZE && y_progress < REGION_HEIGHT-1) {
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
            //layers[layer].vertices.emplace_back(
            //        geometry_t{ 1.0f, (float)tile_x, (float)tile_y, (float)tile_z, (float)width, (float)height, (float)texture_id }
            //);
            add_cube_geometry(layers[layer].v, (float)tile_x, (float)tile_y, (float)tile_z, (float)width, (float)height, (float)texture_id);
            layers[layer].n_elements += 36;
        }
        //std::cout << "Reduced " << n_cubes << " CUBES to " << layers[layer].cube_vertices.size() << " items of geometry.\n";
    }

    void chunk_t::add_floor_geometry(std::vector<float> &v, const float &x, const float &y, const float &z,
                                     const float &width, const float &height, const float &texture_id)
    {
        const float x0 = -0.5f + x;
        const float x1 = x0 + width;
        const float y0 = -0.5f + z;
        //const float y1 = y0 + 1.0f; // We don't use y1 for floors
        const float z0 = -0.5f + y;
        const float z1 = z0 + height;
        const float TI = texture_id;
        constexpr float T0 = 0.0f;
        const float TW = width;
        const float TH = height;
        constexpr float ceiling_gap = 0.001f;

        v.insert(v.end(), {
                // Upwards facing floor
                x1, y0, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f,
                x1, y0, z0, TW, T0, TI,  0.0f,  1.0f,  0.0f,
                x0, y0, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f,
                x0, y0, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f,
                x0, y0, z1, T0, TH, TI,  0.0f,  1.0f,  0.0f,
                x1, y0, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f,
        });
    }

    void chunk_t::add_cube_geometry(std::vector<float> &v, const float &x, const float &y, const float &z,
                                     const float &width, const float &height, const float &texture_id)
    {
        const float x0 = -0.5f + x;
        const float x1 = x0 + width;
        const float y0 = -0.5f + z;
        const float y1 = y0 + 0.98f;
        const float z0 = -0.5f + y;
        const float z1 = z0 + height;
        const float TI = texture_id;
        constexpr float T0 = 0.0f;
        const float TW = width;
        const float TH = height;

        v.insert(v.end(), {
                x0, y0, z0, T0, T0, TI,  0.0f,  0.0f, -1.0f,
                x1, y0, z0, TW, T0, TI,  0.0f,  0.0f, -1.0f,
                x1, y1, z0, TW, TH, TI,  0.0f,  0.0f, -1.0f,
                x1, y1, z0, TW, TH, TI,  0.0f,  0.0f, -1.0f,
                x0, y1, z0, T0, TH, TI,  0.0f,  0.0f, -1.0f,
                x0, y1, z0, T0, TH, TI,  0.0f,  0.0f, -1.0f,

                x0, y0, z1, T0, T0, TI,  0.0f,  0.0f, 1.0f,
                x1, y0, z1, TW, T0, TI,  0.0f,  0.0f, 1.0f,
                x1, y1, z1, TW, TH, TI,  0.0f,  0.0f, 1.0f,
                x1, y1, z1, TW, TH, TI,  0.0f,  0.0f, 1.0f,
                x0, y1, z1, T0, TH, TI,  0.0f,  0.0f, 1.0f,
                x0, y0, z1, T0, T0, TI,  0.0f,  0.0f, 1.0f,

                x0, y1, z1, TW, TH, TI, -1.0f,  0.0f,  0.0f,
                x0, y1, z0, TW, T0, TI, -1.0f,  0.0f,  0.0f,
                x0, y0, z0, T0, T0, TI, -1.0f,  0.0f,  0.0f,
                x0, y0, z0, T0, T0, TI, -1.0f,  0.0f,  0.0f,
                x0, y0, z1, T0, TH, TI, -1.0f,  0.0f,  0.0f,
                x0, y1, z1, TW, TH, TI, -1.0f,  0.0f,  0.0f,

                x1, y1, z1, TW, TH, TI,  1.0f,  0.0f,  0.0f,
                x1, y1, z0, TW, T0, TI,  1.0f,  0.0f,  0.0f,
                x1, y0, z0, T0, T0, TI,  1.0f,  0.0f,  0.0f,
                x1, y0, z0, T0, T0, TI,  1.0f,  0.0f,  0.0f,
                x1, y0, z1, T0, TH, TI,  1.0f,  0.0f,  0.0f,
                x1, y1, z1, TW, TH, TI,  1.0f,  0.0f,  0.0f,

                x0, y0, z0, T0, T0, TI,  0.0f, -1.0f,  0.0f,
                x1, y0, z0, TW, T0, TI,  0.0f, -1.0f,  0.0f,
                x1, y0, z1, TW, TH, TI,  0.0f, -1.0f,  0.0f,
                x1, y0, z1, TW, TH, TI,  0.0f, -1.0f,  0.0f,
                x0, y0, z1, T0, TH, TI,  0.0f, -1.0f,  0.0f,
                x0, y0, z0, T0, T0, TI,  0.0f, -1.0f,  0.0f,

                x0, y1, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f,
                x1, y1, z0, TW, T0, TI,  0.0f,  1.0f,  0.0f,
                x1, y1, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f,
                x1, y1, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f,
                x0, y1, z1, T0, TH, TI,  0.0f,  1.0f,  0.0f,
                x0, y1, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f
        });
    }

	void chunk_t::add_water_geometry(std::vector<float> &v, const float &x, const float &y, const float &z,
		const float &width, const float &height, const float &texture_id, const uint8_t &water_level)
	{
		const float water_y = static_cast<float>(water_level) / 10.0f;
		const float x0 = -0.5f + x;
		const float x1 = x0 + width;
		const float y0 = -0.5f + z + water_y;
		//const float y1 = y0 + 1.0f; // We don't use y1 for floors
		const float z0 = -0.5f + y + water_y;
		const float z1 = z0 + height;
		const float TI = texture_id;
		constexpr float T0 = 0.0f;
		const float TW = width;
		const float TH = height;
		constexpr float ceiling_gap = 0.001f;

		v.insert(v.end(), {
			// Upwards facing floor
			x1, y0, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f,
			x1, y0, z0, TW, T0, TI,  0.0f,  1.0f,  0.0f,
			x0, y0, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f,
			x0, y0, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f,
			x0, y0, z1, T0, TH, TI,  0.0f,  1.0f,  0.0f,
			x1, y0, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f,
		});
	}

	void chunk_t::add_ramp_geometry(std::vector<float> &v, const float &x, const float &y, const float &z,
		const float &width, const float &height, const float &texture_id, const float &ne, const float &se, const float &sw, const float &nw)
	{
		const float x0 = -0.5f + x;
		const float x1 = x0 + width;
		const float y0 = -0.5f + z;
		//const float y1 = y0 + 1.0f; // We don't use y1 for floors
		const float z0 = -0.5f + y;
		const float z1 = z0 + height;
		const float TI = texture_id;
		constexpr float T0 = 0.0f;
		const float TW = width;
		const float TH = height;
		constexpr float ceiling_gap = 0.001f;

		v.insert(v.end(), {
			// Upwards facing floor
			x1, y0 + ne, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f, // NE
			x1, y0 + se, z0, TW, T0, TI,  0.0f,  1.0f,  0.0f, // SE
			x0, y0 + sw, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f, // SW
			x0, y0 + sw, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f, // SW
			x0, y0 + nw, z1, T0, TH, TI,  0.0f,  1.0f,  0.0f, // NW
			x1, y0 + ne, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f, // NE
		});
	}

    void chunk_t::update_buffer() {
        //if (vbo > 0) glDeleteBuffers(1, &vbo);
        //if (vao > 0) glDeleteVertexArrays(1, &vao);

        if (vao < 1) { glGenVertexArrays(1, &vao); glCheckError(); }
        if (vbo < 1) { glGenBuffers(1, &vbo); glCheckError(); }

        // Combine the layers into a temporary structure
        std::vector<float> data;
        for (auto &layer : layers) {
            data.insert(std::end(data), layer.v.begin(), layer.v.end());
            has_geometry = true;
            layer.v.clear();
        }

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0); // 0 = Vertex Position

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (char *) nullptr + 3 * sizeof(float));
        glEnableVertexAttribArray(1); // 1 = TexX/Y/ID

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (char *) nullptr + 6 * sizeof(float));
        glEnableVertexAttribArray(2); // 2 = Normals

        glBindVertexArray(0);
    }

	void chunk_t::update_trans_buffer() {
		if (tvbo > 0) glDeleteBuffers(1, &tvbo);
		if (tvao > 0) glDeleteVertexArrays(1, &tvao);

		if (tvao < 1) { glGenVertexArrays(1, &tvao); glCheckError(); }
		if (tvbo < 1) { glGenBuffers(1, &tvbo); glCheckError(); }

		// Combine the layers into a temporary structure
		std::vector<float> data;
		for (auto &layer : layers) {
			data.insert(std::end(data), layer.trans.begin(), layer.trans.end());
			has_transparency = true;
			layer.trans.clear();
		}
		if (data.size() == 0) return;

		glBindVertexArray(tvao);
		glBindBuffer(GL_ARRAY_BUFFER, tvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, tvbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0); // 0 = Vertex Position

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (char *) nullptr + 3 * sizeof(float));
		glEnableVertexAttribArray(1); // 1 = TexX/Y/ID

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (char *) nullptr + 6 * sizeof(float));
		glEnableVertexAttribArray(2); // 2 = Normals

		glBindVertexArray(0);
	}

    void update_buffers() {
        int idx;        
        {
            std::lock_guard<std::mutex> lock(dirty_buffer_mutex);
            if (dirty_buffers.empty()) return;
            idx = *dirty_buffers.begin();
			chunks[idx].ready.store(false);
            dirty_buffers.erase(idx);
        }
        chunks[idx].update_buffer();
		chunks[idx].update_trans_buffer();
        chunks[idx].ready.store(true);
		std::cout << "Buffer Updated\n";
    }

}