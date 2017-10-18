#include "ai_sleep_time.hpp"
#include "../../../bengine/ecs.hpp"
#include "jobs_board.hpp"
#include "ai_work_template.hpp"
#include "../distance_map_system.hpp"
#include "ai_work_hunt.hpp"
#include "../../../components/ai_tags/ai_tag_work_butcher.hpp"
#include "../../../components/corpse_harvestable.hpp"
#include "../../../components/building.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../../raws/creatures.hpp"
#include "../../../raws/defs/raw_creature_t.hpp"
#include "../inventory_system.hpp"
#include "../distance_map_system.hpp"

namespace systems {
	namespace ai_butcher {

		using namespace bengine;
		using namespace jobs_board;
		using namespace dijkstra;
		using namespace distance_map;

		namespace jobs_board {
			void evaluate_butchering(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (!ai_hunt::jobs_board::butcher_exist()) return;
				const auto distance = butcherables_map.get(mapidx(pos));
				if (distance > MAX_DIJSTRA_DISTANCE - 1) return;
				board.insert(std::make_pair(distance, jt));
			}
		}

		void run(const double &duration_ms) {
			ai_work_template<ai_tag_work_butcher> work;

			work.do_ai([&work](entity_t &e, ai_tag_work_butcher &b, ai_tag_my_turn_t &t, position_t &pos) {
				work.set_status(e, "Butchering");
				if (b.step == ai_tag_work_butcher::butcher_steps::FIND_CORPSE) {
					if (butcherables_map.get(mapidx(pos)) > MAX_DIJSTRA_DISTANCE - 1) {
						work.cancel_work_tag(e);
						return;
					}
					b.step = ai_tag_work_butcher::butcher_steps::GO_TO_CORPSE;
					return;
				}
				else if (b.step == ai_tag_work_butcher::butcher_steps::GO_TO_CORPSE) {
					work.folllow_path(butcherables_map, pos, e, [&e, &work]() {
						// Cancel
						work.cancel_work_tag(e);
					}, [&b]() {
						// Success
						b.step = ai_tag_work_butcher::butcher_steps::COLLECT_CORPSE;
					});
				}
				else if (b.step == ai_tag_work_butcher::butcher_steps::COLLECT_CORPSE) {
					// Find the corpse!
					each<corpse_harvestable, position_t>([&b, &pos](entity_t &E, corpse_harvestable &corpse, position_t &cpos) {
						if (cpos == pos) {
							b.target_corpse = E.id;
						}
					});
					if (b.target_corpse == 0) {
						work.cancel_work_tag(e);
						return;
					}

					// Pick up the corpse
					inventory_system::pickup_item(b.target_corpse, e.id );
					distance_map::refresh_butcherables_map();

					// Find the butcher's shop and go there
					position_t butcher_pos;
					each<building_t, position_t>([&butcher_pos](entity_t &E, building_t &b, position_t &p) {
						if (b.complete == true && b.tag == "butcher") butcher_pos = p;
					});
					b.current_path = find_path(pos, butcher_pos);
					if (!b.current_path) {
						inventory_system::drop_item(b.target_corpse, pos.x, pos.y, pos.z );
						work.cancel_work_tag(e);
						return;
					}
					b.step = ai_tag_work_butcher::butcher_steps::GO_TO_SHOP;
					return;
				}
				else if (b.step == ai_tag_work_butcher::butcher_steps::GO_TO_SHOP) {
					work.follow_path(b, pos, e, [&e, &work, &b, &pos]() {
						// Cancel
						inventory_system::drop_item(b.target_corpse, pos.x, pos.y, pos.z);
						work.cancel_work_tag(e);
					}, [&b]() {
						// Arrived
						b.step = ai_tag_work_butcher::butcher_steps::CHOP;
					});
				}
				else if (b.step == ai_tag_work_butcher::butcher_steps::CHOP) {
					call_home("butcher");
					auto corpse = entity(b.target_corpse)->component<corpse_harvestable>();
					const auto food_idx = get_material_by_tag("food");

					auto finder = get_creature_def(corpse->creature_tag);
					// Spawn bones
					for (int i = 0; i<finder->yield_bone; ++i) {
						auto new_entity = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "bone", food_idx);
						const std::string corpse_type = corpse->creature_tag;
						if (corpse_type != "") {
							auto creature_def = get_creature_def(corpse_type);
							auto item = new_entity->component<item_t>();
							if (creature_def && item) {
								item->item_name = creature_def->name + std::string(" ") + "Bone";
							}
						}
					}

					// Spawn hide
					for (int i = 0; i<finder->yield_hide; ++i) {
						auto new_entity = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "hide", food_idx);
						const std::string corpse_type = corpse->creature_tag;
						if (corpse_type != "") {
							auto creature_def = get_creature_def(corpse_type);
							auto item = new_entity->component<item_t>();
							if (creature_def && item) {
								item->item_name = creature_def->name + std::string(" ") + "Hide";
							}
						}
					}

					// Spawn meat
					for (int i = 0; i<finder->yield_meat; ++i) {
						auto new_entity = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "meat", food_idx);
						const std::string corpse_type = corpse->creature_tag;
						if (corpse_type != "") {
							auto creature_def = get_creature_def(corpse_type);
							auto item = new_entity->component<item_t>();
							if (creature_def && item) {
								item->item_name = creature_def->name + std::string(" ") + "Meat";
							}
						}
					}

					// Spawn skulls
					for (int i = 0; i<finder->yield_skull; ++i) {
						auto new_entity = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "skull", food_idx);
						const std::string corpse_type = corpse->creature_tag;
						if (corpse_type != "") {
							auto creature_def = get_creature_def(corpse_type);
							auto item = new_entity->component<item_t>();
							if (creature_def && item) {
								item->item_name = creature_def->name + std::string(" ") + "Skull";
							}
						}
					}

					delete_entity(b.target_corpse); // Destroy the corpse
					distance_map::refresh_butcherables_map();
					work.cancel_work_tag(e);
					return;
				}
			});
		}
	}
}