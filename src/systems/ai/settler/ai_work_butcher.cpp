#include "stdafx.h"
#include "ai_work_butcher.hpp"
#include "templated_work_steps_t.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../../raws/creatures.hpp"
#include "../../../raws/defs/raw_creature_t.hpp"
#include "../../helpers/targeted_flow_map.hpp"
#include "../distance_map_system.hpp"
#include "../../../raws/raws.hpp"
#include "../../../raws/materials.hpp"
#include "../../../raws/defs/material_def_t.hpp"

namespace systems {
	namespace ai_butcher {

		using namespace bengine;
		using namespace jobs_board;

		static bool butcher_exist() {
			auto butcher_exists = false;

			each<building_t>([&butcher_exists](entity_t &e, building_t &b) {
				if (b.tag == "butcher" && b.complete) butcher_exists = true;
			});

			return butcher_exists;
		}

		namespace jobs_board {
			void evaluate_butchering(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (e.component<designated_hunter_t>() == nullptr) return; // Not a hunter
				if (!butcher_exist()) return;
				if (distance_map::butcher_map->targets.empty()) return; // Nothing to butcher
				board.insert(std::make_pair(15, jt));
			}
		}

		static const char * job_tag = "Butchering";
		static work::templated_work_steps_t<ai_tag_work_butcher> work;

		inline void find_corpse(entity_t &e, ai_tag_work_butcher &b, ai_tag_my_turn_t &t, position_t &pos) {
			if (b.current_path)
			{
				// Follow it
				work.follow_path(b, pos, e, [&e, &b]()
				{
					// Cancel
					b.current_path.reset();
					work.cancel_work_tag(e);
				}, [&b]()
				{
					// Success
					b.current_path.reset();
					b.step = ai_tag_work_butcher::butcher_steps::COLLECT_CORPSE;
				});
			}
			else {
				const auto finder = distance_map::butcher_map->find_nearest_reachable_target(pos);
				if (finder.target == 0)
				{
					// Nothing I can reach
					work.cancel_work_tag(e);
				} else
				{
					b.target_corpse = finder.target;
					b.current_path = finder.path;
				}
			}
		}

		inline void collect_corpse(entity_t &e, ai_tag_work_butcher &b, ai_tag_my_turn_t &t, position_t &pos) {
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
			inventory_system::pickup_item(b.target_corpse, e.id);
			distance_map::refresh_butcherables_map();

			// Find the butcher's shop and go there
			position_t butcher_pos;
			each<building_t, position_t>([&butcher_pos](entity_t &E, building_t &b, position_t &p) {
				if (b.complete && b.tag == "butcher") butcher_pos = p;
			});
			b.current_path = find_path(pos, butcher_pos);
			if (!b.current_path) {
				inventory_system::drop_item(b.target_corpse, pos.x, pos.y, pos.z);
				work.cancel_work_tag(e);
				return;
			}
			b.step = ai_tag_work_butcher::butcher_steps::GO_TO_SHOP;
		}

		inline void goto_shop(entity_t &e, ai_tag_work_butcher &b, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(b, pos, e, [&e, &b, &pos]() {
				// Cancel
				inventory_system::drop_item(b.target_corpse, pos.x, pos.y, pos.z);
				work.cancel_work_tag(e);
			}, [&b]() {
				// Arrived
				b.step = ai_tag_work_butcher::butcher_steps::CHOP;
			});
		}

		inline void chop(entity_t &e, ai_tag_work_butcher &b, ai_tag_my_turn_t &t, position_t &pos) {
			call_home("AI", "Butcher");
			auto corpse = entity(b.target_corpse)->component<corpse_harvestable>();
			const auto food_idx = get_material_by_tag("food");

			auto finder = get_creature_def(corpse->creature_tag);
			std::string cname = "";
			auto name = e.component<name_t>();
			if (name) cname = name->first_name + std::string(" ") + name->last_name;
			// Spawn bones
			for (int i = 0; i<finder->yield_bone; ++i) {
				auto new_entity = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "bone", food_idx, 3, 100, e.id, cname);
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
				auto new_entity = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "hide", food_idx, 3, 100, e.id, cname);
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
				auto new_entity = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "meat", food_idx, 3, 100, e.id, cname);
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
				auto new_entity = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "skull", food_idx, 3, 100, e.id, cname);
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
		}

		void dispatch(entity_t &e, ai_tag_work_butcher &b, ai_tag_my_turn_t &t, position_t &pos) {
			switch (b.step)
			{
			case ai_tag_work_butcher::butcher_steps::FIND_CORPSE : find_corpse(e, b, t, pos); break;
			case ai_tag_work_butcher::butcher_steps::COLLECT_CORPSE: collect_corpse(e, b, t, pos); break;
			case ai_tag_work_butcher::butcher_steps::GO_TO_SHOP: goto_shop(e, b, t, pos); break;
			case ai_tag_work_butcher::butcher_steps::CHOP: chop(e, b, t, pos); break;
			}
		}

		void run(const double &duration_ms) {
			work.do_work(jobs_board::evaluate_butchering, dispatch, job_tag);
		}
	}
}
