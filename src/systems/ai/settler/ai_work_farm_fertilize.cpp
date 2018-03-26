#include "stdafx.h"
#include "ai_work_farm_fertilize.hpp"
#include "templated_work_steps_t.hpp"
#include "../../../global_assets/farming_designations.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../../damage/damage_system.hpp"
#include "../../../render_engine/chunks/chunks.hpp"
#include "ai_work_farm_fertilize.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../../raws/plants.hpp"
#include "../../../raws/defs/plant_t.hpp"
#include "../../helpers/targeted_flow_map.hpp"

namespace systems {
	namespace ai_farm_fertilize {
		using namespace bengine;
		using namespace jobs_board;
		using namespace region;

		namespace jobs_board {
			void evaluate_farm_fertilizing(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (e.component<designated_farmer_t>() == nullptr) return; // Not a farmer
				if (farm_designations->farms.empty()) return; // There are no farms
				if (!inventory::is_item_category_available<item_fertilizer_t>()) return; // No fertilizer

				for (const auto & farm : farm_designations->farms) {
					if (farm.second.state == farm_steps::GROWING && farm.second.fertilized == false) {
						auto[fx, fy, fz] = idxmap(farm.first);
						board.insert(std::make_pair((int)bengine::distance3d(pos.x, pos.y, pos.z, fx, fy, fz), jt));
					}
				}
			}
		}

		static const char * job_tag = "Farm - Fertilize";
		static work::templated_work_steps_t<ai_tag_work_farm_fertilize> work;

		inline void find_hoe(entity_t &e, ai_tag_work_farm_fertilize &h, ai_tag_my_turn_t &t, position_t &pos) {
			work::get_tool<item_farming_t, designated_farmer_t, ai_tag_work_farm_fertilize>(e, work, h, pos,
				[&h]() {
				// On we have it
				h.step = ai_tag_work_farm_fertilize::fertilize_steps::FIND_FERTILZER;
			},
				[&h]() {
				// On need to find it
				h.step = ai_tag_work_farm_fertilize::fertilize_steps::FETCH_HOE;
			});
		}

		inline void fetch_hoe(entity_t &e, ai_tag_work_farm_fertilize &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_farm_fertilize::fertilize_steps::FIND_HOE;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_farm_fertilize::fertilize_steps::FIND_FERTILZER;

				inventory_system::pickup_item(h.tool_id, e.id);
				return;
			});
		}

		inline void find_fertilizer(entity_t &e, ai_tag_work_farm_fertilize &h, ai_tag_my_turn_t &t, position_t &pos) {
			h.current_path.reset();
			auto fertilizer_id = inventory::find_closest_unclaimed_item_by_category_and_claim_it_immediately<item_fertilizer_t>(e.id, pos);
			if (fertilizer_id == 0) {
				work.cancel_work_tag(e);
				return;
			}
			h.fertilizer_id = fertilizer_id;

			h.current_path = find_path(pos, *inventory::get_item_location(fertilizer_id));
			if (!h.current_path->success) {
				work.cancel_work_tag(e);
				return;
			}
			h.step = ai_tag_work_farm_fertilize::fertilize_steps::FETCH_FERTILIZER;
		}

		inline void fetch_fertilizer(entity_t &e, ai_tag_work_farm_fertilize &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_farm_fertilize::fertilize_steps::FIND_HOE;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_farm_fertilize::fertilize_steps::FIND_TARGET;

				inventory_system::pickup_item(h.fertilizer_id, e.id);
				return;
			});
		}

		inline void find_target(entity_t &e, ai_tag_work_farm_fertilize &h, ai_tag_my_turn_t &t, position_t &pos) {
			std::map<int, std::pair<position_t, const farm_cycle_t *>> plant_targets;
			for (const auto &f : farm_designations->farms) {
				if (f.second.state == farm_steps::GROWING && f.second.fertilized == false) {
					auto[X, Y, Z] = idxmap(f.first);
					const float distance = bengine::distance3d(pos.x, pos.y, pos.z, X, Y, Z);
					plant_targets.insert(std::make_pair(static_cast<int>(distance), std::make_pair(position_t{ X, Y, Z }, &f.second)));
				}
			}
			if (plant_targets.empty()) {
				work.cancel_work_tag(e);
				return;
			}

			h.current_path.reset();
			auto[X, Y, Z] = idxmap(plant_targets.begin()->first);
			h.current_path = find_path(pos, position_t{ X, Y, Z });
			if (!h.current_path->success) {
				work.cancel_work_tag(e);
				return;
			}
			h.step = ai_tag_work_farm_fertilize::fertilize_steps::FETCH_TARGET;
		}

		inline void fetch_target(entity_t &e, ai_tag_work_farm_fertilize &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_farm_fertilize::fertilize_steps::FIND_HOE;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_farm_fertilize::fertilize_steps::FERILIZE;
				return;
			});
		}

		inline void fertilize(entity_t &e, ai_tag_work_farm_fertilize &h, ai_tag_my_turn_t &t, position_t &pos) {
			const int idx = mapidx(pos);
			auto farm_finder = farm_designations->farms.find(idx);
			if (farm_finder == farm_designations->farms.end() || farm_finder->second.state != farm_steps::PLANT_SEEDS) {
				//std::cout << "Bailing out - not relevant anymore!";
				inventory_system::drop_item(h.fertilizer_id, pos.x, pos.y, pos.z);
				work.cancel_work_tag(e);
				return;
			}

			if (work::skill_check_or_damage(e, "Farming", DIFFICULTY_AVERAGE, work, 1, "Farming Accident")) {
				// Destroy the fertilizer
				inventory::delete_item(h.fertilizer_id);

				farm_finder->second.fertilized = true;

				// Become idle - done
				call_home("AI", "Fertilize");
				work.cancel_work_tag(e);
				h.step = ai_tag_work_farm_fertilize::fertilize_steps::FIND_HOE;
			}
		}

		void dispatch(entity_t &e, ai_tag_work_farm_fertilize &h, ai_tag_my_turn_t &t, position_t &pos) {
			switch (h.step) {
			case ai_tag_work_farm_fertilize::fertilize_steps::FIND_HOE: find_hoe(e, h, t, pos); break;
			case ai_tag_work_farm_fertilize::fertilize_steps::FETCH_HOE: fetch_hoe(e, h, t, pos); break;
			case ai_tag_work_farm_fertilize::fertilize_steps::FIND_FERTILZER: find_fertilizer(e, h, t, pos); break;
			case ai_tag_work_farm_fertilize::fertilize_steps::FETCH_FERTILIZER: fetch_fertilizer(e, h, t, pos); break;
			case ai_tag_work_farm_fertilize::fertilize_steps::FIND_TARGET: find_target(e, h, t, pos); break;
			case ai_tag_work_farm_fertilize::fertilize_steps::FETCH_TARGET: fetch_target(e, h, t, pos); break;
			case ai_tag_work_farm_fertilize::fertilize_steps::FERILIZE: fertilize(e, h, t, pos); break;
			}
		}

		void run(const double &duration_ms) {
			work.do_work(jobs_board::evaluate_farm_fertilizing, dispatch, job_tag);
		}
	}
}
