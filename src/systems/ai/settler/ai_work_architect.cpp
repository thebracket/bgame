#include "ai_sleep_time.hpp"
#include "../../../bengine/ecs.hpp"
#include "jobs_board.hpp"
#include "ai_work_template.hpp"
#include "../distance_map_system.hpp"
#include "../../../components/ai_tags/ai_tag_work_architect.hpp"
#include "../../physics/movement_system.hpp"
#include "../../../global_assets/game_designations.hpp"
#include "../../../planet/region/region.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../../components/bridge.hpp"
#include "../../../components/receives_signal.hpp"

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
				auto blocks_distance = blocks_map.get(mapidx(pos));
				if (blocks_distance > MAX_DIJSTRA_DISTANCE - 1) return; // Nothing to harvest

				auto build_distance = architecure_map.get(mapidx(pos));
				if (build_distance > MAX_DIJSTRA_DISTANCE - 1) return; // Nothing to harvest

				board.insert(std::make_pair(blocks_distance + build_distance, jt));
			}
		}

		void run(const double &duration_ms) {
			ai_work_template<ai_tag_work_architect> work;
			work.do_ai([&work](entity_t &e, ai_tag_work_architect &a, ai_tag_my_turn_t &t, position_t &pos) {
				work.set_status(e, "Architecture");
				if (a.step == ai_tag_work_architect::architect_steps::GOTO_BLOCK) {
					work.folllow_path(blocks_map, pos, e, [&e, &work]() {
						// Cancel
						work.cancel_work_tag(e);
					}, [&a]() {
						// Success
						a.step = ai_tag_work_architect::architect_steps::COLLECT_BLOCK;
					});
					return;
				}
				else if (a.step == ai_tag_work_architect::architect_steps::COLLECT_BLOCK) {
					std::size_t block_id = 0;
					each<item_t, position_t>([&pos, &block_id](entity_t &E, item_t &i, position_t &block) {
						if (block == pos && i.claimed == false && i.item_tag == "block") {
							block_id = E.id;
						}
					});
					if (block_id == 0) {
						work.cancel_work_tag(e);
						return;
					}
					std::cout << "Collected block\n";
					// TODO: emit(item_claimed_message{ block_id, true });
					// TODO: emit(pickup_item_message{ block_id, e.id });
					// TODO: emit(blocks_changed_message{});
					a.current_tool = block_id;
					a.step = ai_tag_work_architect::architect_steps::GOTO_SITE;
					return;
				}
				else if (a.step == ai_tag_work_architect::architect_steps::GOTO_SITE) {
					const int idx = mapidx(pos);
					const auto distance = architecure_map.get(idx);
					if (distance >= MAX_DIJSTRA_DISTANCE) {
						// TODO: emit(drop_item_message{ a.current_tool, pos.x, pos.y, pos.z });
						work.cancel_work_tag(e);
						return;
					}
					if (distance < 2) {
						a.step = ai_tag_work_architect::architect_steps::BUILD;
						return;
					}
					position_t destination = architecure_map.find_destination(pos);
					move_to(e.id, destination);
					return;
				}
				else if (a.step == ai_tag_work_architect::architect_steps::BUILD) {
					int bidx = 0;

					// Are we standing on the build site, adjacent to it?
					if (designations->architecture.find(mapidx(pos)) != designations->architecture.end()) bidx = mapidx(pos);
					if (bidx == 0 && designations->architecture.find(mapidx(pos.x - 1, pos.y, pos.z)) != designations->architecture.end()) bidx = mapidx(pos.x - 1, pos.y, pos.z);
					if (bidx == 0 && designations->architecture.find(mapidx(pos.x + 1, pos.y, pos.z)) != designations->architecture.end()) bidx = mapidx(pos.x + 1, pos.y, pos.z);
					if (bidx == 0 && designations->architecture.find(mapidx(pos.x, pos.y - 1, pos.z)) != designations->architecture.end()) bidx = mapidx(pos.x, pos.y - 1, pos.z);
					if (bidx == 0 && designations->architecture.find(mapidx(pos.x, pos.y + 1, pos.z)) != designations->architecture.end()) bidx = mapidx(pos.x, pos.y + 1, pos.z);

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
						// TODO: emit(destroy_item_message{ a.current_tool });
						a.current_tool = 0;

						set_flag(bidx, CONSTRUCTION);
						if (material > 0) set_tile_material(bidx, material);

						if (build_type == 0) {
							// Wall
							make_wall(bidx, material);
						}
						else if (build_type == 1) {
							// Floor
							make_floor(bidx);
						}
						else if (build_type == 2) {
							// Up
							make_stairs_up(bidx);
						}
						else if (build_type == 3) {
							// Down
							make_stairs_down(bidx);
						}
						else if (build_type == 4) {
							// UpDown
							make_stairs_updown(bidx);
						}
						else if (build_type == 5) {
							// Ramp
							make_ramp(bidx);
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
						}

						int cx, cy, cz;
						std::tie(cx, cy, cz) = idxmap(bidx);
						tile_calculate(cx, cy, cz);
						for (int Z = -2; Z<3; ++Z) {
							for (int Y = -2; Y<3; ++Y) {
								for (int X = -2; X<3; ++X) {
									tile_calculate(cx + X, cy + Y, cz + Z);
								}
							}
						}

						designations->architecture.erase(bidx);
						// TODO: emit(architecture_changed_message{});
						// TODO: emit(renderables_changed_message{});
						// TODO: emit(map_changed_message{});
						call_home("architecture", std::to_string(build_type));
					}
					else {
						// TODO: emit(drop_item_message{ a.current_tool, pos.x, pos.y, pos.z });
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