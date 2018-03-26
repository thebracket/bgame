#include "stdafx.h"
#include "ai_work_farm_water.hpp"
#include "templated_work_steps_t.hpp"
#include "../../../global_assets/farming_designations.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../../damage/damage_system.hpp"
#include "../../../render_engine/chunks/chunks.hpp"
#include "ai_work_farm_water.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../helpers/targeted_flow_map.hpp"

namespace systems {
	namespace ai_farm_water {
		using namespace bengine;
		using namespace jobs_board;
		using namespace region;

		namespace jobs_board {
			void evaluate_farm_watering(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (e.component<designated_farmer_t>() == nullptr) return; // Not a farmer
				if (farm_designations->farms.empty()) return; // There are no farms

				for (const auto & farm : farm_designations->farms) {
					if (farm.second.state == farm_steps::GROWING && farm.second.days_since_watered > 0) {
						auto[fx, fy, fz] = idxmap(farm.first);
						board.insert(std::make_pair((int)bengine::distance3d(pos.x, pos.y, pos.z, fx, fy, fz), jt));
					}
				}
			}
		}

		static const char * job_tag = "Farm - Water Crops";
		static work::templated_work_steps_t<ai_tag_work_farm_water> work;

		inline void find_hoe(entity_t &e, ai_tag_work_farm_water &h, ai_tag_my_turn_t &t, position_t &pos) {
			work::get_tool<item_farming_t, designated_farmer_t, ai_tag_work_farm_water>(e, work, h, pos,
				[&h]() {
				// On we have it
				h.step = ai_tag_work_farm_water::water_steps::FIND_TARGET;
			},
				[&h]() {
				// On need to find it
				h.step = ai_tag_work_farm_water::water_steps::FETCH_HOE;
			});
		}

		inline void fetch_hoe(entity_t &e, ai_tag_work_farm_water &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_farm_water::water_steps::FIND_HOE;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_farm_water::water_steps::FIND_TARGET;

				inventory_system::pickup_item(h.tool_id, e.id);
				return;
			});
		}

		inline void find_target(entity_t &e, ai_tag_work_farm_water &h, ai_tag_my_turn_t &t, position_t &pos) {
			std::map<int, std::pair<position_t, const farm_cycle_t *>> plant_targets;
			for (const auto &f : farm_designations->farms) {
				if (f.second.state == farm_steps::GROWING && f.second.days_since_watered > 0) {
					auto[X, Y, Z] = idxmap(f.first);
					//std::cout << X << "/" << Y << "/" << Z << "\n";
					const float distance = bengine::distance3d(pos.x, pos.y, pos.z, X, Y, Z);
					plant_targets.insert(std::make_pair(static_cast<int>(distance), std::make_pair(position_t{ X, Y, Z }, &f.second)));
				}
			}
			if (plant_targets.empty()) {
				work.cancel_work_tag(e);
				return;
			}

			h.current_path.reset();
			h.current_path = find_path(pos, plant_targets.begin()->second.first);
			if (!h.current_path->success) {
				work.cancel_work_tag(e);
				return;
			}
			h.step = ai_tag_work_farm_water::water_steps::FETCH_TARGET;
		}

		inline void fetch_target(entity_t &e, ai_tag_work_farm_water &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_farm_water::water_steps::FIND_HOE;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_farm_water::water_steps::WATER;
				return;
			});
		}

		inline void water(entity_t &e, ai_tag_work_farm_water &h, ai_tag_my_turn_t &t, position_t &pos) {
			const int idx = mapidx(pos);

			auto farm_finder = farm_designations->farms.find(idx);
			if (farm_finder == farm_designations->farms.end() || farm_finder->second.state != farm_steps::GROWING || farm_finder->second.days_since_watered == 0) {
				//std::cout << "Bailing out - not relevant anymore!";
				work.cancel_work_tag(e);
				return;
			}

			if (work::skill_check_or_damage(e, "Farming", DIFFICULTY_AVERAGE, work, 1, "Farming Accident")) {
				farm_finder->second.days_since_watered = 0;
			}
		}

		void dispatch(entity_t &e, ai_tag_work_farm_water &h, ai_tag_my_turn_t &t, position_t &pos) {
			switch (h.step) {
			case ai_tag_work_farm_water::water_steps::FIND_HOE : find_hoe(e, h, t, pos); break;
			case ai_tag_work_farm_water::water_steps::FETCH_HOE: fetch_hoe(e, h, t, pos); break;
			case ai_tag_work_farm_water::water_steps::FIND_TARGET: find_target(e, h, t, pos); break;
			case ai_tag_work_farm_water::water_steps::FETCH_TARGET: fetch_target(e, h, t, pos); break;
			case ai_tag_work_farm_water::water_steps::WATER: water(e, h, t, pos); break;
			}
		}

		void run(const double &duration_ms) {
			work.do_work(jobs_board::evaluate_farm_watering, dispatch, job_tag);
		}
	}
}
