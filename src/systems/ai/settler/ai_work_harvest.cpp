#include "stdafx.h"
#include "ai_work_harvest.hpp"
#include "templated_work_steps_t.hpp"
#include "../distance_map_system.hpp"
#include "../../../planet/region/region.hpp"
#include "../../../raws/plants.hpp"
#include "../../../raws/defs/plant_t.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../../../bengine/geometry.hpp"
#include "../../../render_engine/chunks/chunks.hpp"
#include "../../damage/damage_system.hpp"
#include "../../../global_assets/farming_designations.hpp"
#include "../../../raws/defs/item_def_t.hpp"
#include "../../../raws/raws.hpp"
#include "../../../raws/materials.hpp"
#include "../../../raws/defs/material_def_t.hpp"
#include "../../../raws/items.hpp"

namespace systems {
	namespace ai_harvest {

		using namespace bengine;
		using namespace jobs_board;
		using namespace distance_map;
		using namespace dijkstra;
		using namespace region;

		namespace jobs_board {
			void evaluate_harvest(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (e.component<designated_farmer_t>() == nullptr) return; // Not a farmer
				if (farm_designations->harvest.empty()) return; // Nothing to harvest

				board.insert(std::make_pair((int)bengine::distance3d(pos.x, pos.y, pos.z, farm_designations->harvest.begin()->second.x, farm_designations->harvest.begin()->second.y, farm_designations->harvest.begin()->second.z), jt));
			}
		}

		static const char * job_tag = "Farm - Harvest";
		static work::templated_work_steps_t<ai_tag_work_harvest> work;

		inline void find_hoe(entity_t &e, ai_tag_work_harvest &h, ai_tag_my_turn_t &t, position_t &pos) {
			work::get_tool<item_farming_t, designated_farmer_t, ai_tag_work_harvest>(e, work, h, pos,
				[&h]() {
				// On we have it
				h.step = ai_tag_work_harvest::harvest_steps::FIND_HARVEST;
			},
				[&h]() {
				// On need to find it
				h.step = ai_tag_work_harvest::harvest_steps::FETCH_HOE;
			});
		}

		inline void fetch_hoe(entity_t &e, ai_tag_work_harvest &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_harvest::harvest_steps::FIND_HOE;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_harvest::harvest_steps::FIND_HARVEST;

				inventory_system::pickup_item(h.tool_id, e.id);
				return;
			});
		}

		inline void find_harvest(entity_t &e, ai_tag_work_harvest &h, ai_tag_my_turn_t &t, position_t &pos) {
			std::map<int, position_t> harvest_targets;
			for (const auto ht : farm_designations->harvest) {
				const float distance = bengine::distance3d(pos.x, pos.y, pos.z, ht.second.x, ht.second.y, ht.second.z);
				harvest_targets.insert(std::make_pair(static_cast<int>(distance), ht.second));
			}
			h.current_path.reset();
			bool done = false;
			auto iterator = harvest_targets.begin();
			while (!done && !h.current_path) {
				h.current_path = find_path(pos, iterator->second);
				if (!h.current_path->success) {
					++iterator;
					if (iterator == harvest_targets.end()) done = true;
				}
				else {
					h.step = ai_tag_work_harvest::harvest_steps::GOTO_HARVEST;
					return;
				}
			}
			if (!h.current_path) {
				work.cancel_work_tag(e);
				return;
			}
		}

		inline void goto_harvest(entity_t &e, ai_tag_work_harvest &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_harvest::harvest_steps::FIND_HARVEST;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_harvest::harvest_steps::DO_HARVEST;
				return;
			});
			return;
		}

		inline void do_harvest(entity_t &e, ai_tag_work_harvest &h, ai_tag_my_turn_t &t, position_t &pos) {
			const int idx = mapidx(pos);
			farm_designations->harvest.erase(std::remove_if(
				farm_designations->harvest.begin(),
				farm_designations->harvest.end(),
				[&idx](std::pair<bool, position_t> p) { return idx == mapidx(p.second); }
			),
				farm_designations->harvest.end());

			if (work::skill_check_or_damage<ai_tag_work_harvest>(e, "Farming", DIFFICULTY_EASY, work, 1, "Farming Accident")) {
				auto farm_finder = farm_designations->farms.find(idx);

				// Create the harvesting result
				if (veg_type(idx) == 0) {
					work.cancel_work_tag(e);
					return;
				}
				std::string cname = "";
				auto name = e.component<name_t>();
				if (name) cname = name->first_name + std::string(" ") + name->last_name;
				const auto plant = get_plant_def(veg_type(idx));
				const std::string result = plant->provides[veg_lifecycle(idx)];
				if (result != "none") {
					std::string mat_type = "organic";
					auto item_finder = get_item_def(result);
					if (item_finder != nullptr) {
						if (item_finder->categories.test(ITEM_FOOD)) mat_type = "food";
						if (item_finder->categories.test(ITEM_SPICE)) mat_type = "spice";
					}
					int n_spawn = 1;
					if (farm_finder != farm_designations->farms.end()) {
						const int n = farm_finder->second.fertilized ? 4 : 2;
						n_spawn = rng.roll_dice(n, 4);
						farm_finder->second.state = farm_steps::GROWING;
						farm_finder->second.fertilized = false;

					}
					for (int i = 0; i < n_spawn; ++i) {
						auto item = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, result, get_material_by_tag(mat_type), 3, 100, e.id, cname);
						item->component<item_t>()->item_name = plant->name;
					}
				}

				// Spawn seeds
				const int n_seeds = rng.roll_dice(1, 4) + 1;
				for (int i = 0; i < n_seeds; ++i) {
					auto item = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "seed", get_material_by_tag("organic"), 3, 100, e.id, cname);
					item->component<item_t>()->item_name = plant->name + std::string(" Seed");
					item->component<item_seed_t>()->grows_into = plant->tag;
				}

				// Knock tile back to germination
				set_veg_lifecycle(idx, 0);
				set_veg_ticker(idx, 0);
				chunks::mark_chunk_dirty_by_tileidx(idx);

				// Become idle - done
				call_home("AI", "Harvest", result);
				work.cancel_work_tag(e);
				h.step = ai_tag_work_harvest::harvest_steps::FIND_HOE;
			}
		}

		void dispatch(entity_t &e, ai_tag_work_harvest &h, ai_tag_my_turn_t &t, position_t &pos) {
			switch (h.step) {
			case ai_tag_work_harvest::harvest_steps::FIND_HOE: find_hoe(e, h, t, pos); break;
			case ai_tag_work_harvest::harvest_steps::FETCH_HOE: fetch_hoe(e, h, t, pos); break;
			case ai_tag_work_harvest::harvest_steps::FIND_HARVEST: find_harvest(e, h, t, pos); break;
			case ai_tag_work_harvest::harvest_steps::GOTO_HARVEST: goto_harvest(e, h, t, pos); break;
			case ai_tag_work_harvest::harvest_steps::DO_HARVEST: do_harvest(e, h, t, pos); break;
			}
		}

		void run(const double &duration_ms) {
			work.do_work(jobs_board::evaluate_harvest, dispatch, job_tag);
		}
	}
}
