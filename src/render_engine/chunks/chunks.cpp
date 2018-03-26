#include "chunks.hpp"
//#include "../../bengine/threadpool.h"
#include "../../planet/indices.hpp"
#include "../../planet/region/region.hpp"
#include "../../raws/materials.hpp"
#include "geometry_helper.hpp"
#include "../../global_assets/farming_designations.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../global_assets/game_ecs.hpp"

using namespace tile_flags;

namespace chunks {
    std::array<chunk_t, CHUNKS_TOTAL> chunks;
    bool chunks_initialized = false;

    // Dirty chunks
    static std::vector<bool> dirty;
    //std::mutex dirty_mutex;

    // Vertex geometry updates; need to not be multi-threaded
    static std::set<int, std::greater<int>> dirty_buffers;
    //std::mutex dirty_buffer_mutex;

	void mark_chunk_dirty_by_tileidx(const int &idx) {
		const auto &[x,y,z] = idxmap(idx);
		mark_chunk_dirty(chunk_idx(x / CHUNK_SIZE, y / CHUNK_SIZE, z / CHUNK_SIZE));
	}

    void mark_chunk_dirty(const int &idx) {
        //std::lock_guard<std::mutex> lock(dirty_mutex);
        dirty[idx] = true;
    }

    void mark_chunk_clean(const int &idx) {
        //std::lock_guard<std::mutex> lock(dirty_mutex);
        dirty[idx] = false;
    }

    void enqueue_vertex_update(const int &idx) {
        //std::lock_guard<std::mutex> lock(dirty_buffer_mutex);
        dirty_buffers.insert(idx);
		mark_chunk_clean(idx);
		//std::cout << "Enqueued buffer update\n";
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

    static void update_chunk(int thread_num, const int &idx) {
        //chunks[idx].ready.store(false);
        chunks[idx].update();
    }

    void update_dirty_chunks() {
        //std::lock_guard<std::mutex> lock(dirty_mutex);
        for (int i=0; i<CHUNKS_TOTAL; ++i) {
            if (dirty[i]) {
                dirty[i] = false;
                //chunks[i].ready.store(false);
                //bengine::thread_pool->push(update_chunk, i);
				update_chunk(0, i);
            }
        }
    }        
    
    void chunk_t::update() {
        //dirty[index] = false; // We're guaranteed to already be mutex-protected

        for (auto &layer : layers) {
            layer.v.clear();
            layer.n_elements = 0;
        }
		static_voxel_models.clear();

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
                        if (region::flag(idx, REVEALED)) {
							if (tiletype == tile_type::WINDOW) {
								// TODO: Windows go into transparency buffer
								cubes[idx] = 15;
							} else if (tiletype == tile_type::FLOOR) {
                                floors[idx] = get_floor_tex(idx);
								if (farm_designations->farms.find(idx) != farm_designations->farms.end()) {
									static_voxel_models[116].push_back(std::make_tuple(region_x, region_y, region_z));
								}
                            } else if (is_cube(tiletype)) {
                                cubes[idx] = get_cube_tex(idx);
							} else if (tiletype == tile_type::RAMP) {
								// TODO - write me!
								float ne = 0.0f, se = 0.0f, sw = 0.0f, nw = 0.0f;
								if (region::flag(idx - 1, SOLID)) { sw = 1.0f; nw = 1.0f; }
								else if (region::flag(idx + 1, SOLID)) { se = 1.0f; ne = 1.0f; }
								else if (region::flag(idx + REGION_WIDTH, SOLID)) { nw = 1.0f; ne = 1.0f; }
								else if (region::flag(idx - REGION_WIDTH, SOLID)) { sw = 1.0f; se = 1.0f; }
								layers[chunk_z].n_elements += add_ramp_geometry(layers[chunk_z].v, region_x, region_y, region_z, 1.0f, 1.0f, get_floor_tex(idx), ne, se, sw, nw);
							}
							else if (tiletype == tile_type::STAIRS_DOWN) {
								static_voxel_models[24].push_back(std::make_tuple(region_x, region_y, region_z));
							}
							else if (tiletype == tile_type::STAIRS_UP) {
								static_voxel_models[23].push_back(std::make_tuple(region_x, region_y, region_z));
							}
							else if (tiletype == tile_type::STAIRS_UPDOWN) {
								static_voxel_models[25].push_back(std::make_tuple(region_x, region_y, region_z));
							}
							else if (tiletype == tile_type::CLOSED_DOOR) {
								auto vox_id = 128;
								const auto bid = region::get_building_id(idx);
								if (bid > 0)
								{
									const auto building_entity = bengine::entity(bid);
									if (building_entity)
									{
										const auto building_comp = building_entity->component<building_t>();
										if (building_comp)
										{
											const auto def = get_building_def(building_comp->tag);
											if (def)
											{
												for (const auto &p : def->provides)
												{
													if (p.alternate_vox > 0) vox_id = p.alternate_vox;
												}
											}
										}
									}
								}
								static_voxel_models[vox_id].push_back(std::make_tuple(region_x, region_y, region_z));
							}
						}
                    }
                }
            }

            if (!floors.empty()) greedy_floors(floors, chunk_z);
            if (!cubes.empty()) greedy_cubes(cubes, chunk_z);

            if (!floors.empty() || !cubes.empty()) layer_requires_render.set(chunk_z);
        }

        has_geometry = layer_requires_render.count() > 0;

        // Tell GL to update
        enqueue_vertex_update(index);
    }

    void chunk_t::greedy_floors(boost::container::flat_map<int, unsigned int> &floors, const int &layer) {
        auto n_floors = floors.size();

        //std::cout << floors.size() << " floors to process\n";

        while (!floors.empty()) {
            const auto first_floor = floors.begin();
            const auto base_region_idx = first_floor->first;
            const auto texture_id = first_floor->second;

			const auto &[tile_x, tile_y, tile_z] = idxmap(base_region_idx);
            auto width = 1;
            auto height = 1;

            floors.erase(base_region_idx);
            auto idx_grow_right = base_region_idx+1;
            auto x_coordinate = tile_x;
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
                auto y_progress = tile_y + 1;

				auto possible = true;
                while (possible && y_progress < base_y + CHUNK_SIZE && y_progress < REGION_HEIGHT-1) {
                    for (auto gx = tile_x; gx < tile_x + width; ++gx) {
                        const auto candidate_idx = mapidx(gx, y_progress, tile_z);
                        const auto vfinder = floors.find(candidate_idx);
                        if (vfinder == floors.end() || vfinder->second != texture_id) possible = false;
                    }
                    if (possible) {
                        ++height;
                        for (auto gx = tile_x; gx < tile_x + width; ++gx) {
                            const auto candidate_idx = mapidx(gx, y_progress, tile_z);
                            floors.erase(candidate_idx);
                        }
                    }

                    ++y_progress;
                }
            }
            //std::cout << "Merging with " << height << " tiles vertically\n";

            // TODO: Emit geometry
            //std::cout << floors.size() << " floors remain\n";
			layers[layer].n_elements += add_floor_geometry(layers[layer].v, (float)tile_x, (float)tile_y, (float)tile_z, (float)width, (float)height, (float)texture_id);
            //layers[layer].vertices.emplace_back(
            //        geometry_t{ 0.0f, (float)tile_x, (float)tile_y, (float)tile_z, (float)width, (float)height, (float)texture_id }
            //);
        }
        //std::cout << "Reduced " << n_floors << " FLOORS to " << layers[layer].floor_vertices.size() << " items of geometry.\n";
    }

    void chunk_t::greedy_cubes(boost::container::flat_map<int, unsigned int> &cubes, const int &layer) {
		auto n_cubes = cubes.size();

        while (!cubes.empty()) {
            const auto first_floor = cubes.begin();
            const auto base_region_idx = first_floor->first;
            const auto texture_id = first_floor->second;

			const auto &[tile_x, tile_y, tile_z] = idxmap(base_region_idx);
			auto width = 1;
			auto height = 1;

            cubes.erase(base_region_idx);
			auto idx_grow_right = base_region_idx+1;
			auto x_coordinate = tile_x;
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
				auto y_progress = tile_y + 1;

				auto possible = true;
                while (possible && y_progress < base_y + CHUNK_SIZE && y_progress < REGION_HEIGHT-1) {
                    for (auto gx = tile_x; gx < tile_x + width; ++gx) {
                        const auto candidate_idx = mapidx(gx, y_progress, tile_z);
                        const auto vfinder = cubes.find(candidate_idx);
                        if (!(vfinder != cubes.end() && vfinder->second == texture_id)) possible = false;
                    }
                    if (possible) {
                        ++height;
                        for (int gx = tile_x; gx < tile_x + width; ++gx) {
                            const auto candidate_idx = mapidx(gx, y_progress, tile_z);
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
			layers[layer].n_elements += add_cube_geometry(layers[layer].v, (float)tile_x, (float)tile_y, (float)tile_z, (float)width, (float)height, (float)texture_id);
        }
        //std::cout << "Reduced " << n_cubes << " CUBES to " << layers[layer].cube_vertices.size() << " items of geometry.\n";
    }       
	
    void chunk_t::update_buffer() {
		auto reset_vao = false;
        if (vao < 1) { 
			glGenVertexArrays(1, &vao); 
			glCheckError(); 
			reset_vao = true;
		}
        if (vbo < 1) { 
			glGenBuffers(1, &vbo); 
			glCheckError(); 
		}

        // Combine the layers into a temporary structure
        std::vector<float> data;
        for (auto &layer : layers) {
            data.insert(std::end(data), layer.v.begin(), layer.v.end());
            has_geometry = true;
            layer.v.clear();
			layer.v.shrink_to_fit();
        }

		//glInvalidateBufferData(vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_STATIC_DRAW);

		if (reset_vao) {
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0); // 0 = Vertex Position, using the last vector element to be real Z

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char *) nullptr + 4 * sizeof(float));
			glEnableVertexAttribArray(1); // 1 = TexX/Y/ID

			glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char *) nullptr + 7 * sizeof(float));
			glEnableVertexAttribArray(2); // 2 = Normals

			glBindVertexArray(0);
			glCheckError();
		}
    }

	bool made_flags_ssbo = false;
	unsigned int flags_ssbo;

	static void update_world_buffer()
	{
		if (!made_flags_ssbo)
		{
			glGenBuffers(1, &flags_ssbo);
			made_flags_ssbo = true;

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, flags_ssbo);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * region::get_tile_flags()->size(), &region::get_tile_flags()->operator[](0), GL_DYNAMIC_COPY);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		} else
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, flags_ssbo);
			GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
			memcpy(p, &region::get_tile_flags()->operator[](0), sizeof(uint32_t) * region::get_tile_flags()->size());
			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		}
		
	}

    void update_buffers() {
		bool did_something = false;
		while (!dirty_buffers.empty()) {
			int idx;
			idx = *dirty_buffers.begin();
			dirty_buffers.erase(idx);
			chunks[idx].update_buffer();
			did_something = true;
		}
		update_world_buffer(); // Always so that view sheds are visible
    }

}
