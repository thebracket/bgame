#include "door_system.hpp"
#include "../../bengine/ecs.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../planet/region/region.hpp"
#include "../../components/item.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../components/entry_trigger.hpp"
#include "../../components/receives_signal.hpp"
#include "../../render_engine/vox/renderables.hpp"
#include "trigger_system.hpp"
#include "../../render_engine/chunks/chunks.hpp"
#include "../gui/particle_system.hpp"
#include "../ai/mining_system.hpp"
#include "../ai/distance_map_system.hpp"

using namespace bengine;

namespace systems {
	namespace topology {

		using namespace bengine;
		using namespace region;

		struct perform_construction_message {
			perform_construction_message() {}
			perform_construction_message(const std::size_t &entity, const std::string &t, const std::size_t &mat)
				: entity_id(entity), tag(t), material(mat) {}

			std::size_t entity_id;
			std::string tag;
			std::size_t material;
		};

		struct perform_mining_message {
			perform_mining_message() {}
			perform_mining_message(const int idx, const uint8_t op, int X, int Y, int Z) : target_idx(idx),
				operation(op), x(X), y(Y), z(Z) {}
			int target_idx;
			uint8_t operation;
			int x, y, z;
		};

		thread_safe_message_queue<perform_construction_message> construction;
		thread_safe_message_queue<perform_mining_message> mining;

		void perform_mining(const int idx, const int op, int X, int Y, int Z) {
			mining.enqueue(perform_mining_message{ idx, static_cast<uint8_t>(op), X, Y, Z });
		}

		void perform_construction(const size_t &entity, const std::string &t, const std::size_t &mat) {
			construction.enqueue(perform_construction_message{ entity, t, mat });
		}

		void dig(const perform_mining_message &e) {
			make_floor(e.target_idx);
			//auto &[x,y,z] = idxmap(e.target_idx);
			// TODO: emit_deferred(tile_removed_message{ x,y,z });
		}

		void channel(const perform_mining_message &e) {
			make_open_space(e.target_idx);
			//auto &[x,y,z] = idxmap(e.target_idx);
			// TODO: emit_deferred(tile_removed_message{ x,y,z });

			// Add ramp
			const int below = e.target_idx - (REGION_WIDTH * REGION_HEIGHT);
			if (solid(below)) {
				make_ramp(below);
			}
		}

		void ramp(const perform_mining_message &e) {
			make_ramp(e.target_idx);

			const int above = e.target_idx + (REGION_WIDTH * REGION_HEIGHT);
			if (solid(above)) {
				make_open_space(above);
				set_flag(above, CAN_STAND_HERE);
			}
		}

		void stairs_up(const perform_mining_message &e) {
			make_stairs_up(e.target_idx);
		}

		void stairs_down(const perform_mining_message &e) {
			make_stairs_down(e.target_idx);
		}

		void stairs_updown(const perform_mining_message &e) {
			make_stairs_updown(e.target_idx);
		}

		void recalculate(const perform_mining_message &e) {
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

		void spawn_mining_result_impl(const perform_mining_message &e, std::string tag) {
			if (tag == "") return;

			auto [X,Y,Z] = idxmap(e.target_idx);

			auto finder = get_item_def(tag);
			if (finder != nullptr) {
				//std::cout << "Topology system - producing a [" << tag << "]";
				auto mat = material(e.target_idx);
				if (tag == "ore") {
					for (const std::string &metal : get_material(mat)->ore_materials) {
						const auto metal_finder = get_material_by_tag(metal);
						spawn_item_on_ground(X, Y, Z, tag, metal_finder);
					}
				}
				else {
					spawn_item_on_ground(X, Y, Z, tag, mat);
				}
			}
			else {
				std::cout << "Topology system - don't know how to spawn a [" << tag << "]\n";
			}
		}

		void spawn_mining_result(const perform_mining_message &e) {
			const std::string mining_result = get_material(material(e.target_idx))->mines_to_tag;
			const std::string mining_result2 = get_material(material(e.target_idx))->mines_to_tag_second;

			//std::cout << "Topology - mines to [" << mining_result << "], [" << mining_result2 << "]";

			spawn_mining_result_impl(e, mining_result);
			spawn_mining_result_impl(e, mining_result2);
		}

		void build_construction(const perform_construction_message &e) {
			std::cout << "Received a build construction message\n";
			std::cout << "Entity ID: " << e.entity_id << "\n";
			std::cout << "Tag: " << e.tag << "\n";
			std::cout << "Material: " << e.material << "\n";

			bool entity_should_be_deleted = true;
			auto construction_pos = entity(e.entity_id)->component<position_t>();
			const int index = mapidx(construction_pos->x, construction_pos->y, construction_pos->z);
			auto finder = get_building_def(e.tag);
			for (const building_provides_t &provides : finder->provides) {

				if (provides.provides == provides_wall) {
					set_tile_type(index, tile_type::WALL);
					// Relocate anything stuck in the new wall
					each<position_t>([index](entity_t &E, position_t &P) {
						if (mapidx(P.x, P.y, P.z) == index) {
							// Something needs moving!
							if (!solid(index + 1)) {
								std::tie(P.x, P.y, P.z) = idxmap(index + 1);
							}
							else if (!solid(index - 1)) {
								std::tie(P.x, P.y, P.z) = idxmap(index - 1);
							}
							else if (!solid(index + REGION_WIDTH)) {
								std::tie(P.x, P.y, P.z) = idxmap(index + REGION_WIDTH);
							}
							else if (!solid(index - REGION_WIDTH)) {
								std::tie(P.x, P.y, P.z) = idxmap(index - REGION_WIDTH);
							}
						}
					});
				}
				else if (provides.provides == provides_floor) {
					set_tile_type(index, tile_type::FLOOR);
				}
				else if (provides.provides == provides_stairs_up) {
					set_tile_type(index, tile_type::STAIRS_UP);
				}
				else if (provides.provides == provides_stairs_down) {
					set_tile_type(index, tile_type::STAIRS_DOWN);
				}
				else if (provides.provides == provides_stairs_updown) {
					set_tile_type(index, tile_type::STAIRS_UPDOWN);
				}
				else if (provides.provides == provides_ramp) {
					set_tile_type(index, tile_type::RAMP);
				}
				else if (provides.provides == provides_cage_trap) {
					// Create a new entity for the trap
					// Add an entry_trigger and a position to it
					auto [x,y,z] = idxmap(index);
					create_entity()->assign(position_t{ x, y, z })->assign(entry_trigger_t{ trigger_cage });
					// TODO: emit_deferred(triggers_changes_message{});
					entity_should_be_deleted = false;
				}
				else if (provides.provides == provides_stonefall_trap) {
					// Create a new entity for the trap
					// Add an entry_trigger and a position to it
					auto &[x,y,z] = idxmap(index);
					create_entity()->assign(position_t{ x, y, z })->assign(entry_trigger_t{ trigger_stonefall });
					// TODO: emit_deferred(triggers_changes_message{});
					entity_should_be_deleted = false;
				}
				else if (provides.provides == provides_blades_trap) {
					// Create a new entity for the trap
					// Add an entry_trigger and a position to it
					auto &[x,y,z] = idxmap(index);
					create_entity()->assign(position_t{ x, y, z })->assign(entry_trigger_t{ trigger_blade });
					// TODO: emit_deferred(triggers_changes_message{});
					entity_should_be_deleted = false;
				}
				else if (provides.provides == provides_spikes) {
					// Create a new entity for the trap
					// Add an entry_trigger and a position to it
					auto [x,y,z] = idxmap(index);
					create_entity()->assign(position_t{ x, y, z })->assign(receives_signal_t{});
					entity_should_be_deleted = false;
				}
			}
			set_tile_material(index, e.material);

			tile_calculate(construction_pos->x, construction_pos->y, construction_pos->z);
			for (int Z = -2; Z<3; ++Z) {
				for (int Y = -2; Y<3; ++Y) {
					for (int X = -2; X<3; ++X) {
						tile_calculate(construction_pos->x + X, construction_pos->y + Y, construction_pos->z + Z);
					}
				}
			}
			particles::block_destruction_effect(construction_pos->x, construction_pos->y, construction_pos->z, 1.0f, 1.0f, 1.0f, particles::PARTICLE_LUMBERJACK);

			if (entity_should_be_deleted) {
				delete_entity(e.entity_id);
				std::cout << "Deleted entity\n";
			}
			// TODO: emit(renderables_changed_message{});
			render::models_changed = true;
			chunks::mark_chunk_dirty_by_tileidx(index);
			// TODO: emit(map_changed_message{});
		}

		void run(const double &duration_ms) {
			mining.process_all([](const perform_mining_message &e) {
				switch (e.operation) {
				case 1: dig(e); break;
				case 2: channel(e); break;
				case 3: ramp(e); break;
				case 4: stairs_up(e); break;
				case 5: stairs_down(e); break;
				case 6: stairs_updown(e); break;
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
			});

			construction.process_all(build_construction);
		}
	}
}