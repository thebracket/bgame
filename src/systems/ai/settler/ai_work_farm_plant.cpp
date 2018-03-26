#include "stdafx.h"
#include "ai_work_farm_plant.hpp"
#include "templated_work_steps_t.hpp"
#include "../../../global_assets/farming_designations.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../../damage/damage_system.hpp"
#include "../../../render_engine/chunks/chunks.hpp"
#include "ai_work_farm_plant.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../../raws/plants.hpp"
#include "../../../raws/defs/plant_t.hpp"
#include "../../../planet/region/region.hpp"

namespace systems {
	namespace ai_farm_plant {
		using namespace bengine;
		using namespace jobs_board;
		using namespace region;

		namespace jobs_board {
			void evaluate_farm_planting(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (e.component<designated_farmer_t>() == nullptr) return; // Not a farmer
				if (farm_designations->farms.empty()) return; // There are no farms
				for (const auto & farm : farm_designations->farms) {
					if (farm.second.state == farm_steps::PLANT_SEEDS) {
						auto[fx, fy, fz] = idxmap(farm.first);
						board.insert(std::make_pair((int)bengine::distance3d(pos.x, pos.y, pos.z, fx, fy, fz), jt));
					}
				}
			}
		}

		static const char * job_tag = "Farm - Planting";
		static work::templated_work_steps_t<ai_tag_work_farm_plant> work;

		inline void find_hoe(entity_t &e, ai_tag_work_farm_plant &h, ai_tag_my_turn_t &t, position_t &pos) {
			work::get_tool<item_farming_t, designated_farmer_t, ai_tag_work_farm_plant>(e, work, h, pos,
				[&h]() {
				// On we have it
				h.step = ai_tag_work_farm_plant::plant_steps::FIND_SEED;
			},
				[&h]() {
				// On need to find it
				h.step = ai_tag_work_farm_plant::plant_steps::FETCH_HOE;
			});
		}

		inline void fetch_hoe(entity_t &e, ai_tag_work_farm_plant &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_farm_plant::plant_steps::FIND_HOE;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_farm_plant::plant_steps::FIND_SEED;

				inventory_system::pickup_item(h.tool_id, e.id);
				return;
			});
		}

		inline void find_seed(entity_t &e, ai_tag_work_farm_plant &h, ai_tag_my_turn_t &t, position_t &pos) {
			// Determine what farm we are working on
			std::map<int, std::pair<position_t, const farm_cycle_t *>> plant_targets;
			for (const auto &f : farm_designations->farms) {
				if (f.second.state == farm_steps::PLANT_SEEDS) {
					auto[X, Y, Z] = idxmap(f.first);
					const float distance = bengine::distance3d(pos.x, pos.y, pos.z, X, Y, Z);
					plant_targets.insert(std::make_pair(static_cast<int>(distance), std::make_pair(position_t{ X, Y, Z }, &f.second)));
				}
			}
			if (plant_targets.empty()) {
				work.cancel_work_tag(e);
				return;
			}
			h.farm_position = plant_targets.begin()->second.first;

			// This tells us the seed type!
			if (plant_targets.begin()->second.second->seed_id > 0) {
				h.seed_id = plant_targets.begin()->second.second->seed_id;
			}
			else {
				// Find a seed for this farm
				const auto seed_type = plant_targets.begin()->second.second->seed_type;
				//std::cout << seed_type << "\n";
				std::size_t seed_id = 0;
				each<claimed_t, item_seed_t>([&seed_type, &seed_id](entity_t &se, claimed_t &claim, item_seed_t &seed) {
					//std::cout << seed.grows_into << "\n";
					if (seed.grows_into == seed_type) seed_id = se.id;
				});
				if (seed_id == 0) {
					work.cancel_work_tag(e);
					return;
				}
				return;
			}

			h.current_path.reset();
			h.current_path = find_path(pos, *inventory::get_item_location(h.seed_id));
			if (!h.current_path->success) {
				work.cancel_work_tag(e);
				return;
			}
			h.step = ai_tag_work_farm_plant::plant_steps::FETCH_SEED;
		}

		inline void fetch_seed(entity_t &e, ai_tag_work_farm_plant &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_farm_plant::plant_steps::FIND_HOE;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_farm_plant::plant_steps::FIND_TARGET;

				inventory_system::pickup_item(h.seed_id, e.id);
				return;
			});
		}

		inline void find_target(entity_t &e, ai_tag_work_farm_plant &h, ai_tag_my_turn_t &t, position_t &pos) {
			h.current_path.reset();
			h.current_path = find_path(pos, h.farm_position);
			if (!h.current_path->success) {
				work.cancel_work_tag(e);
				return;
			}
			h.step = ai_tag_work_farm_plant::plant_steps::FETCH_TARGET;
		}

		inline void fetch_target(entity_t &e, ai_tag_work_farm_plant &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_farm_plant::plant_steps::FIND_HOE;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_farm_plant::plant_steps::PLANT;
				return;
			});
		}

		inline void plant(entity_t &e, ai_tag_work_farm_plant &h, ai_tag_my_turn_t &t, position_t &pos) {
			const int idx = mapidx(pos);

			auto farm_finder = farm_designations->farms.find(idx);
			if (farm_finder == farm_designations->farms.end() || farm_finder->second.state != farm_steps::PLANT_SEEDS) {
				//std::cout << "Bailing out - not relevant anymore!";
				inventory_system::drop_item(h.seed_id, pos.x, pos.y, pos.z);
				work.cancel_work_tag(e);
				return;
			}

			if (work::skill_check_or_damage(e, "Farming", DIFFICULTY_AVERAGE, work, 1, "Farming Accident")) {
				// Plant and update plant info
				set_veg_type(idx, get_plant_idx(entity(h.seed_id)->component<item_seed_t>()->grows_into));
				set_veg_lifecycle(idx, 0);
				set_veg_ticker(idx, 0);
				chunks::mark_chunk_dirty_by_tileidx(idx);
				farm_finder->second.seed_id = 0;
				farm_finder->second.state = farm_steps::GROWING;

				// Destroy the seed
				inventory::delete_item(h.seed_id);

				// Become idle - done
				call_home("AI", "Plant");
				work.cancel_work_tag(e);
				h.step = ai_tag_work_farm_plant::plant_steps::FIND_HOE;
			}
		}

		void dispatch(entity_t &e, ai_tag_work_farm_plant &h, ai_tag_my_turn_t &t, position_t &pos) {
			switch (h.step) {
			case ai_tag_work_farm_plant::plant_steps::FIND_HOE : find_hoe(e, h, t, pos); break;
			case ai_tag_work_farm_plant::plant_steps::FETCH_HOE: fetch_hoe(e, h, t, pos); break;
			case ai_tag_work_farm_plant::plant_steps::FIND_SEED: find_seed(e, h, t, pos); break;
			case ai_tag_work_farm_plant::plant_steps::FETCH_SEED: fetch_seed(e, h, t, pos); break;
			case ai_tag_work_farm_plant::plant_steps::FIND_TARGET: find_target(e, h, t, pos); break;
			case ai_tag_work_farm_plant::plant_steps::FETCH_TARGET: fetch_target(e, h, t, pos); break;
			case ai_tag_work_farm_plant::plant_steps::PLANT: plant(e, h, t, pos); break;
			}
		}

		void run(const double &duration_ms) {
			work.do_work(jobs_board::evaluate_farm_planting, dispatch, job_tag);
		}
	}
}
