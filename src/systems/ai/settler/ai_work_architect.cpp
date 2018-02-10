#include "ai_work_architect.hpp"
#include "jobs_board.hpp"
#include "ai_work_template.hpp"
#include "../../../components/ai_tags/ai_tag_work_architect.hpp"
#include "../../../global_assets/game_designations.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../../components/buildings/bridge.hpp"
#include "../../../components/buildings/receives_signal.hpp"
#include "../../../components/claimed_t.hpp"
#include "../../../render_engine/chunks/chunks.hpp"
#include "../mining_system.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "templated_work_steps_t.hpp"
#include "../architecture_system.hpp"

namespace systems {
	namespace ai_architect {

		using namespace bengine;
		using namespace jobs_board;
		using namespace dijkstra;
		using namespace distance_map;
		using namespace movement;
		using namespace region;

		namespace jobs_board {
			void evaluate_architecture(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {

				if (designations->architecture.empty()) return;
				const auto build_distance = architecture_system::architecture_map[mapidx(pos)].distance;
				if (build_distance == std::numeric_limits<uint8_t>::max()) return; // Nothing to build

				if (inventory::blocks_available() == 0) return; // No blocks

				board.insert(std::make_pair(build_distance, jt));
			}
		}

		static bool first_run = true;

		void run(const double &duration_ms) {
			using namespace architecture_system;

			if (first_run) {
				first_run = false;
				register_job_offer<ai_tag_work_architect>(jobs_board::evaluate_architecture);
			}

			ai_work_template<ai_tag_work_architect> work;
			work.do_ai("Architecture", [&work](entity_t &e, ai_tag_work_architect &a, ai_tag_my_turn_t &t, position_t &pos) {
				if (a.step == ai_tag_work_architect::architect_steps::GOTO_BLOCK) {
					if (!a.current_path) {
						// Find the closest block
						std::map<int, size_t> block_distances;
						each_without<claimed_t, item_t>([&pos, &block_distances](entity_t &item_entity, item_t &item)
						{
							if (item.item_tag == "block") {
								auto item_loc = inventory::get_item_location(item_entity.id);
								if (item_loc) {
									block_distances.insert(std::make_pair(static_cast<int>(distance3d(pos.x, pos.y, pos.z, item_loc->x, item_loc->y, item_loc->z)), item_entity.id));
								}
							}
						});

						// Claim it
						if (block_distances.empty())
						{
							work.cancel_work_tag(e);
							return;
						}
						const auto item_entity = entity(block_distances.begin()->second);
						if (!item_entity)
						{
							work.cancel_work_tag(e);
							return;
						}
						item_entity->assign(claimed_t{ e.id });
						a.current_tool = item_entity->id;

						// Path to it
						a.current_path = find_path(pos, *inventory::get_item_location(item_entity->id));
						if (!a.current_path || !a.current_path->success)
						{
							a.current_path.reset();
							work.cancel_work_tag(e);
							return;
						}
					}

					work.follow_path(a, pos, e, [&work, &e, &pos, &a]()
					{
						// Cancel
						a.current_path.reset();
						if (a.current_tool > 0) {
							inventory_system::drop_item(a.current_tool, pos.x, pos.y, pos.z);
							inventory_system::claim_item(a.current_tool, false);
						}
						work.cancel_work_tag(e);
					}, [&a]()
					{
						// Success
						a.step = ai_tag_work_architect::architect_steps::COLLECT_BLOCK;
					});
					return;
				}
				else if (a.step == ai_tag_work_architect::architect_steps::COLLECT_BLOCK) {
					a.current_path.reset();
					inventory_system::pickup_item(a.current_tool, e.id);
					inventory_system::claim_item(a.current_tool, false);
					a.step = ai_tag_work_architect::architect_steps::GOTO_SITE;
					return;
				}
				else if (a.step == ai_tag_work_architect::architect_steps::GOTO_SITE) {
					const auto idx = mapidx(pos.x, pos.y, pos.z);
					if (architecture_map[idx].distance == 0) {
						// We're at a minable site
						a.step = ai_tag_work_architect::architect_steps::BUILD;
						return;
					}
					else if (architecture_map[idx].distance == std::numeric_limits<uint8_t>::max()) {
						// There's nothing to do - someone else must have done it.
						std::cout << "Cancelling because of lack of architecture tasks\n";
						inventory_system::pickup_item(a.current_tool, e.id);
						inventory_system::claim_item(a.current_tool, false);
						work.cancel_work_tag(e);
						return;
					}
					else {
						// Path towards the work
						int current_direction = 0;
						uint8_t min_value = std::numeric_limits<uint8_t>::max();
						if (architecture_map[mapidx(pos.x, pos.y - 1, pos.z)].distance < min_value && flag(idx, CAN_GO_NORTH)) {
							min_value = architecture_map[mapidx(pos.x, pos.y - 1, pos.z)].distance;
							current_direction = 1;
						}
						if (architecture_map[mapidx(pos.x, pos.y + 1, pos.z)].distance < min_value && flag(idx, CAN_GO_SOUTH)) {
							min_value = architecture_map[mapidx(pos.x, pos.y + 1, pos.z)].distance;
							current_direction = 2;
						}
						if (architecture_map[mapidx(pos.x - 1, pos.y, pos.z)].distance < min_value && flag(idx, CAN_GO_WEST)) {
							min_value = architecture_map[mapidx(pos.x - 1, pos.y, pos.z)].distance;
							current_direction = 3;
						}
						if (architecture_map[mapidx(pos.x + 1, pos.y, pos.z)].distance < min_value && flag(idx, CAN_GO_EAST)) {
							min_value = architecture_map[mapidx(pos.x + 1, pos.y, pos.z)].distance;
							current_direction = 4;
						}
						if (architecture_map[mapidx(pos.x, pos.y, pos.z - 1)].distance < min_value && flag(idx, CAN_GO_DOWN)) {
							min_value = architecture_map[mapidx(pos.x, pos.y, pos.z - 1)].distance;
							current_direction = 5;
						}
						if (architecture_map[mapidx(pos.x, pos.y, pos.z + 1)].distance < min_value && flag(idx, CAN_GO_UP)) {
							min_value = architecture_map[mapidx(pos.x, pos.y, pos.z + 1)].distance;
							current_direction = 6;
						}

						if (current_direction == 0) {
							std::cout << "Direction 0 - drop tools\n";
							work.cancel_work_tag(e);
							return;
						}

						std::cout << "Direction: " << current_direction << "\n";
						position_t destination = pos;
						switch (current_direction) {
						case 1: --destination.y; break;
						case 2: ++destination.y; break;
						case 3: --destination.x; break;
						case 4: ++destination.x; break;
						case 5: --destination.z; break;
						case 6: ++destination.z; break;
						}
						movement::move_to(e.id, destination);
						std::cout << "Emitted entity movement - " << e.id << "\n";
					}
					return;
				}
				else if (a.step == ai_tag_work_architect::architect_steps::BUILD) {
					const int idx = mapidx(pos);
					int bidx = architecture_map[idx].target;

					auto finder = designations->architecture.find(bidx);
					if (finder != designations->architecture.end()) {
						uint8_t build_type = finder->second;

						std::size_t material = 0;
						auto block_e = entity(a.current_tool);
						if (block_e) {
							auto Item = block_e->component<item_t>();
							if (Item) {
								material = Item->material;
							}
						}
						inventory_system::destroy_item(a.current_tool);
						a.current_tool = 0;

						set_flag(bidx, CONSTRUCTION);
						if (material > 0) set_tile_material(bidx, material);

						if (build_type == 0) {
							// Wall
							make_wall(bidx, material);
							call_home("AI", "Architecture", "Wall");
						}
						else if (build_type == 1) {
							// Floor
							make_floor(bidx);
							call_home("AI", "Architecture", "Floor");
						}
						else if (build_type == 2) {
							// Up
							make_stairs_up(bidx);
							call_home("AI", "Architecture", "Up");
						}
						else if (build_type == 3) {
							// Down
							make_stairs_down(bidx);
							call_home("AI", "Architecture", "Down");
						}
						else if (build_type == 4) {
							// UpDown
							make_stairs_updown(bidx);
							call_home("AI", "Architecture", "UpDown");
						}
						else if (build_type == 5) {
							// Ramp
							make_ramp(bidx);
							call_home("AI", "Architecture", "Ramp");
						}
						else if (build_type == 6) {
							make_floor(bidx);

							// We need to iterate through the bridge tiles and see if it is done yet.
							const auto bid = bridge_id(bidx);
							bool complete = true;
							int bridge_idx = 0;
							each_bridge([&bridge_idx, &bid, &bidx, &complete](std::size_t id) {
								if (id == bid && bridge_idx != bidx && designations->architecture.find(bridge_idx) != designations->architecture.end()) {
									complete = false;
								}
								++bridge_idx;
							});
							if (complete) {
								entity(bid)->component<bridge_t>()->complete = true;
								entity(bid)->assign(receives_signal_t{});
							}
							call_home("AI", "Architecture", "Bridge");
						}

						auto [cx, cy, cz] = idxmap(bidx);
						tile_calculate(cx, cy, cz);
						for (int Z = -2; Z<3; ++Z) {
							for (int Y = -2; Y<3; ++Y) {
								for (int X = -2; X<3; ++X) {
									tile_calculate(cx + X, cy + Y, cz + Z);
								}
							}
						}
						chunks::mark_chunk_dirty_by_tileidx(bidx);

						designations->architecture.erase(bidx);
						mining_system::mining_map_changed();
						architecture_map_changed();
						distance_map::refresh_all_distance_maps();
						render::models_changed = true;
						// TODO: emit(map_changed_message{});
						call_home("architecture", std::to_string(build_type));
					}
					else {
						inventory_system::drop_item(a.current_tool, pos.x, pos.y, pos.z );
						inventory_system::claim_item(a.current_tool, false);
						work.cancel_work_tag(e);
						return;
					}
					work.cancel_work_tag(e);
					return;
				}
			});
		}
	}
}
