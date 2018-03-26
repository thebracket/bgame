#include "stdafx.h"
#include "ai_work_farm_clear.hpp"
#include "templated_work_steps_t.hpp"
#include "../../../global_assets/farming_designations.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../../damage/damage_system.hpp"
#include "../../../render_engine/chunks/chunks.hpp"
#include "../../../raws/materials.hpp"
#include "../../../raws/defs/material_def_t.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../../raws/plants.hpp"
#include "../../../raws/defs/plant_t.hpp"
#include "../../../raws/items.hpp"
#include "../../../raws/defs/item_def_t.hpp"
#include "../../../raws/raws.hpp"
#include "../../helpers/targeted_flow_map.hpp"

namespace systems {
	namespace ai_farm_clear {

		using namespace bengine;
		using namespace jobs_board;
		using namespace region;

		namespace jobs_board {
			void evaluate_farm_clearance(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (e.component<designated_farmer_t>() == nullptr) return; // Not a farmer
				if (farm_designations->farms.empty()) return; // There are no farms
				for (const auto & farm : farm_designations->farms) {
					if (farm.second.state == farm_steps::CLEAR) {
						auto[fx, fy, fz] = idxmap(farm.first);
						board.insert(std::make_pair((int)bengine::distance3d(pos.x, pos.y, pos.z, fx, fy, fz), jt));
					}
				}
			}
		}

		static const char * job_tag = "Farm - Plough";
		static work::templated_work_steps_t<ai_tag_work_farm_clear> work;

		inline void find_hoe(entity_t &e, ai_tag_work_farm_clear &h, ai_tag_my_turn_t &t, position_t &pos) {
			work::get_tool<item_farming_t, designated_farmer_t, ai_tag_work_farm_clear>(e, work, h, pos,
				[&h]() {
				// On we have it
				h.step = ai_tag_work_farm_clear::clear_steps::FIND_TARGET;
			},
				[&h]() {
				// On need to find it
				h.step = ai_tag_work_farm_clear::clear_steps::FETCH_HOE;
			});
		}

		inline void fetch_hoe(entity_t &e, ai_tag_work_farm_clear &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_farm_clear::clear_steps::FIND_HOE;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_farm_clear::clear_steps::FIND_TARGET;

				inventory_system::pickup_item(h.tool_id, e.id);
				return;
			});
		}

		inline void find_target(entity_t &e, ai_tag_work_farm_clear &h, ai_tag_my_turn_t &t, position_t &pos) {
			std::map<int, position_t> clear_targets;
			for (const auto &f : farm_designations->farms) {
				if (f.second.state == farm_steps::CLEAR) {
					auto[X, Y, Z] = idxmap(f.first);
					const float distance = bengine::distance3d(pos.x, pos.y, pos.z, X, Y, Z);
					clear_targets.insert(std::make_pair(static_cast<int>(distance), position_t{ X, Y, Z }));
				}
			}

			h.current_path.reset();
			bool done = false;
			auto iterator = clear_targets.begin();
			while (!done && !h.current_path) {
				h.current_path = find_path(pos, iterator->second);
				if (!h.current_path->success) {
					++iterator;
					if (iterator == clear_targets.end()) done = true;
				}
				else {
					h.step = ai_tag_work_farm_clear::clear_steps::GOTO_TARGET;
					return;
				}
			}
			if (!h.current_path) {
				work.cancel_work_tag(e);
				return;
			}
		}

		inline void goto_target(entity_t &e, ai_tag_work_farm_clear &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_farm_clear::clear_steps::FIND_TARGET;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_farm_clear::clear_steps::CLEAR_TARGET;
				return;
			});
		}

		inline void clear_target(entity_t &e, ai_tag_work_farm_clear &h, ai_tag_my_turn_t &t, position_t &pos) {
			const int idx = mapidx(pos);

			auto farm_finder = farm_designations->farms.find(idx);
			if (farm_finder == farm_designations->farms.end() || farm_finder->second.state != farm_steps::CLEAR) {
				//std::cout << "Bailing out - not relevant anymore!";
				work.cancel_work_tag(e);
				return;
			}

			if (work::skill_check_or_damage(e, "Farming", DIFFICULTY_EASY, work, 1, "Farming Accident")) {
				// Harvest anything here
				if (veg_type(idx) > 0) {
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
						auto item = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, result, get_material_by_tag(mat_type), 3, 100, e.id, cname);
						item->component<item_t>()->item_name = plant->name;
					}

					// Spawn seeds
					const int n_seeds = rng.roll_dice(1, 4) + 1;
					for (int i = 0; i < n_seeds; ++i) {
						auto item = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "seed", get_material_by_tag("organic"), 3, 100, e.id, cname);
						item->component<item_t>()->item_name = plant->name + std::string(" Seed");
						item->component<item_seed_t>()->grows_into = plant->tag;
					}
				}

				// Set the tile to not have a plant
				set_veg_type(idx, 0);
				set_veg_lifecycle(idx, 0);
				set_veg_ticker(idx, 0);
				chunks::mark_chunk_dirty_by_tileidx(idx);

				auto tm = get_material(region::material(idx));
				if (tm && (tm->spawn_type == SOIL || tm->spawn_type == SAND)) {
					farm_finder->second.state = farm_steps::PLANT_SEEDS;
				}
				else {
					farm_finder->second.state = farm_steps::FIX_SOIL;
				}

				// Become idle - done
				call_home("AI", "Plough");
				work.cancel_work_tag(e);
				h.step = ai_tag_work_farm_clear::clear_steps::FIND_HOE;
			}
		}

		void dispatch(entity_t &e, ai_tag_work_farm_clear &h, ai_tag_my_turn_t &t, position_t &pos) {
			switch (h.step) {
			case ai_tag_work_farm_clear::clear_steps::FIND_HOE : find_hoe(e, h, t, pos); break;
			case ai_tag_work_farm_clear::clear_steps::FETCH_HOE: fetch_hoe(e, h, t, pos); break;
			case ai_tag_work_farm_clear::clear_steps::FIND_TARGET: find_target(e, h, t, pos); break;
			case ai_tag_work_farm_clear::clear_steps::GOTO_TARGET: goto_target(e, h, t, pos); break;
			case ai_tag_work_farm_clear::clear_steps::CLEAR_TARGET: clear_target(e, h, t, pos); break;
			}
		}

		void run(const double &duration_ms) {
			work.do_work(jobs_board::evaluate_farm_clearance, dispatch, job_tag);
		}
	}
}
