#include "stdafx.h"
#include "topology_system.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../planet/region/region.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../render_engine/vox/renderables.hpp"
#include "../../render_engine/chunks/chunks.hpp"
#include "../gui/particle_system.hpp"
#include "../ai/mining_system.hpp"
#include "../ai/distance_map_system.hpp"
#include "../ai/architecture_system.hpp"
#include "gravity_system.hpp"

using namespace bengine;
using namespace tile_flags;

namespace systems {
	namespace topology {

		using namespace bengine;
		using namespace region;

		struct perform_construction_message {
			perform_construction_message() = default;
			perform_construction_message(const std::size_t &entity, const std::string &t, const std::size_t &mat)
				: entity_id(entity), tag(t), material(mat) {}

			std::size_t entity_id = 0;
			std::string tag;
			std::size_t material = 0;
		};		

		static thread_safe_message_queue<perform_construction_message> construction;
		static thread_safe_message_queue<perform_mining_message> mining;

		void perform_mining(const int idx, const int op, const int x, const int y, const int z) {
			mining.enqueue(perform_mining_message{ idx, static_cast<uint8_t>(op), x, y, z });
		}

		void perform_construction(const size_t &entity, const std::string &t, const std::size_t &mat) {
			construction.enqueue(perform_construction_message{ entity, t, mat });
		}

		static void dig(const perform_mining_message &e) {
			make_floor(e.target_idx);
			//auto &[x,y,z] = idxmap(e.target_idx);
			gravity::tile_was_removed();
		}

		static void channel(const perform_mining_message &e) {
			make_open_space(e.target_idx);

			// Add ramp
			const auto below = e.target_idx - (REGION_WIDTH * REGION_HEIGHT);
			if (flag(below, SOLID)) {
				make_ramp(below);
			}
			gravity::tile_was_removed();
		}

		static void ramp(const perform_mining_message &e) {
			make_ramp(e.target_idx);

			const auto above = e.target_idx + (REGION_WIDTH * REGION_HEIGHT);
			if (flag(above, SOLID)) {
				make_open_space(above);
				set_flag(above, CAN_STAND_HERE);
			}
			gravity::tile_was_removed();
		}

		static void stairs_up(const perform_mining_message &e) {
			make_stairs_up(e.target_idx);
			gravity::tile_was_removed();
		}

		static void stairs_down(const perform_mining_message &e) {
			make_stairs_down(e.target_idx);
			gravity::tile_was_removed();
		}

		static void stairs_updown(const perform_mining_message &e) {
			make_stairs_updown(e.target_idx);
			gravity::tile_was_removed();
		}

		static void recalculate(const perform_mining_message &e) {
			for (int Z = -2; Z<3; ++Z) {
				for (int Y = -2; Y<3; ++Y) {
					for (int X = -2; X<3; ++X) {
						if (e.x + X > 0 && e.x + X < REGION_WIDTH && e.y + Y > 0 && e.y + Y < REGION_HEIGHT && e.z + Z > 0 && e.z + Z<REGION_DEPTH) {
							reveal(mapidx(e.x + X, e.y + Y, e.z + Z));
							tile_calculate(e.x + X, e.y + Y, e.z + Z);
							chunks::mark_chunk_dirty_by_tileidx(mapidx(e.x + X, e.y + Y, e.z + Z));
						}
					}
				}
			}
		}

		static void spawn_mining_result_impl(const perform_mining_message &e, const std::string &tag) {
			if (tag.empty()) return;

			auto [X,Y,Z] = idxmap(e.target_idx);

			const auto finder = get_item_def(tag);
			if (finder != nullptr) {
				const auto mat = material(e.target_idx);
				if (tag == "ore") {
					for (const auto &metal : get_material(mat)->ore_materials) {
						const auto metal_finder = get_material_by_tag(metal);
						spawn_item_on_ground(X, Y, Z, tag, metal_finder, 3, 100, 0, "Mining");
					}
				}
				else {
					spawn_item_on_ground(X, Y, Z, tag, mat, 3, 100, 0, "Mining");
				}
			}
			else {
				glog(log_target::GAME, log_severity::warning, "Topology system - don't know how to spawn a [{0}]", tag);
			}
		}

		void spawn_mining_result(const perform_mining_message &e) {
			const auto mining_result = get_material(material(e.target_idx))->mines_to_tag;
			const auto mining_result2 = get_material(material(e.target_idx))->mines_to_tag_second;

			spawn_mining_result_impl(e, mining_result);
			spawn_mining_result_impl(e, mining_result2);
		}

		static void build_construction(const perform_construction_message &e) {
			auto entity_should_be_deleted = true;
			const auto construction_pos = entity(e.entity_id)->component<position_t>();
			const auto index = mapidx(construction_pos->x, construction_pos->y, construction_pos->z);
			auto finder = get_building_def(e.tag);
			for (const auto &provides : finder->provides) {

				if (provides.provides == provides_wall) {
					set_tile_type(index, tile_type::WALL);
					set_tile_material(index, e.material);
					// Relocate anything stuck in the new wall
					each<position_t>([index](entity_t &E, position_t &P) {
						if (mapidx(P.x, P.y, P.z) == index) {
							// Something needs moving!
							if (!flag(index + 1, SOLID)) {
								std::tie(P.x, P.y, P.z) = idxmap(index + 1);
							}
							else if (!flag(index - 1, SOLID)) {
								std::tie(P.x, P.y, P.z) = idxmap(index - 1);
							}
							else if (!flag(index + REGION_WIDTH, SOLID)) {
								std::tie(P.x, P.y, P.z) = idxmap(index + REGION_WIDTH);
							}
							else if (!flag(index - REGION_WIDTH, SOLID)) {
								std::tie(P.x, P.y, P.z) = idxmap(index - REGION_WIDTH);
							}
						}
					});
				}
				else if (provides.provides == provides_floor) {
					set_tile_type(index, tile_type::FLOOR);
					set_tile_material(index, e.material);
				}
				else if (provides.provides == provides_stairs_up) {
					set_tile_type(index, tile_type::STAIRS_UP);
					set_tile_material(index, e.material);
				}
				else if (provides.provides == provides_stairs_down) {
					set_tile_type(index, tile_type::STAIRS_DOWN);
					set_tile_material(index, e.material);
				}
				else if (provides.provides == provides_stairs_updown) {
					set_tile_type(index, tile_type::STAIRS_UPDOWN);
					set_tile_material(index, e.material);
				}
				else if (provides.provides == provides_ramp) {
					set_tile_type(index, tile_type::RAMP);
					set_tile_material(index, e.material);
				}
			}

			tile_calculate(construction_pos->x, construction_pos->y, construction_pos->z);
			for (auto inner_z = -2; inner_z<3; ++inner_z) {
				for (auto inner_y = -2; inner_y<3; ++inner_y) {
					for (auto inner_x = -2; inner_x<3; ++inner_x) {
						tile_calculate(construction_pos->x + inner_x, construction_pos->y + inner_y, construction_pos->z + inner_z);
					}
				}
			}
			particles::block_destruction_effect(construction_pos->x, construction_pos->y, construction_pos->z, 1.0f, 1.0f, 1.0f, particles::PARTICLE_LUMBERJACK);

			if (entity_should_be_deleted) {
				delete_entity(e.entity_id);
				//std::cout << "Deleted entity\n";
			}
			render::models_changed = true;
			chunks::mark_chunk_dirty_by_tileidx(index);
		}

		void run(const double &duration_ms) {
			mining.process_all([](const perform_mining_message &e) {
				switch (e.operation) {
				case MINE_DIG: dig(e); break;
				case MINE_CHANNEL: channel(e); break;
				case MINE_RAMP: ramp(e); break;
				case MINE_STAIRS_UP: stairs_up(e); break;
				case MINE_STAIRS_DOWN: stairs_down(e); break;
				case MINE_STAIRS_UPDOWN: stairs_updown(e); break;
				default: throw std::runtime_error("Unknown mining operation: " + std::to_string(e.operation));
				}
				spawn_mining_result(e);
				recalculate(e);
				auto[x, y, z] = idxmap(e.target_idx);
				particles::block_destruction_effect(x, y, z, 1.0f, 1.0f, 1.0f, particles::PARTICLE_SMOKE);

				std::vector<int> target_chunks{ e.target_idx };
				if (x > 0) target_chunks.emplace_back(e.target_idx - 1);
				if (x > 1) target_chunks.emplace_back(e.target_idx - 2);
				if (x < REGION_WIDTH - 1) target_chunks.emplace_back(e.target_idx + 1);
				if (x < REGION_WIDTH - 2) target_chunks.emplace_back(e.target_idx + 2);
				if (y > 0) target_chunks.emplace_back(e.target_idx - REGION_WIDTH);
				if (y > 1) target_chunks.emplace_back(e.target_idx - REGION_WIDTH - REGION_WIDTH);
				if (y < REGION_HEIGHT - 1) target_chunks.emplace_back(e.target_idx + REGION_WIDTH);
				if (y < REGION_HEIGHT - 2) target_chunks.emplace_back(e.target_idx + REGION_WIDTH + REGION_WIDTH);
				if (x > 0 && y > 0) target_chunks.emplace_back(mapidx(x - 1, y - 1, z));
				if (x < REGION_WIDTH-1 && y > 0) target_chunks.emplace_back(mapidx(x + 1, y - 1, z));
				if (x < REGION_WIDTH - 1 && y < REGION_HEIGHT-1) target_chunks.emplace_back(mapidx(x + 1, y + 1, z));
				if (x > 0 && y < REGION_HEIGHT-1) target_chunks.emplace_back(mapidx(x - 1, y + 1, z));
				if (z > 0) target_chunks.emplace_back(mapidx(x, y, z - 1));
				if (z < REGION_DEPTH-1) target_chunks.emplace_back(mapidx(x, y, z + 1));

				for (const auto &idx : target_chunks) {
					region::reveal(idx);
					chunks::mark_chunk_dirty_by_tileidx(idx);
				}
				mining_system::mining_map_changed();
				distance_map::refresh_all_distance_maps();
				architecture_system::architecture_map_changed();
			});

			construction.process_all(build_construction);
		}
	}
}