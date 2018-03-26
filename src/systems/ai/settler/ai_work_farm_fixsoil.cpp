#include "stdafx.h"
#include "ai_work_farm_fixsoil.hpp"
#include "templated_work_steps_t.hpp"
#include "../../../global_assets/farming_designations.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../../../render_engine/chunks/chunks.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../helpers/targeted_flow_map.hpp"

namespace systems {
	namespace ai_farm_fixsoil {
		using namespace bengine;
		using namespace jobs_board;
		using namespace region;

		namespace jobs_board {
			void evaluate_farm_fixing(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
				if (e.component<designated_farmer_t>() == nullptr) return; // Not a farmer
				if (farm_designations->farms.empty()) return; // There are no farms
				if (!inventory::is_item_category_available<item_topsoil_t>()) return; // No available soil
				for (const auto & farm : farm_designations->farms) {
					if (farm.second.state == farm_steps::FIX_SOIL) {
						auto[fx, fy, fz] = idxmap(farm.first);
						board.insert(std::make_pair((int)bengine::distance3d(pos.x, pos.y, pos.z, fx, fy, fz), jt));
					}
				}
			}
		}

		static const char * job_tag = "Farm - Planting";
		static work::templated_work_steps_t<ai_tag_work_farm_fixsoil> work;

		inline void find_hoe(entity_t &e, ai_tag_work_farm_fixsoil &h, ai_tag_my_turn_t &t, position_t &pos) {
			work::get_tool<item_farming_t, designated_farmer_t, ai_tag_work_farm_fixsoil>(e, work, h, pos,
				[&h]() {
				// On we have it
				h.step = ai_tag_work_farm_fixsoil::fix_steps::FIND_SOIL;
			},
				[&h]() {
				// On need to find it
				h.step = ai_tag_work_farm_fixsoil::fix_steps::FETCH_HOE;
			});
		}

		inline void fetch_hoe(entity_t &e, ai_tag_work_farm_fixsoil &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_farm_fixsoil::fix_steps::FIND_HOE;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_farm_fixsoil::fix_steps::FIND_SOIL;

				inventory_system::pickup_item(h.tool_id, e.id);
				return;
			});
		}

		inline void find_soil(entity_t &e, ai_tag_work_farm_fixsoil &h, ai_tag_my_turn_t &t, position_t &pos) {
			if (!inventory::is_item_category_available<item_topsoil_t>()) {
				// No soil available - bail out
				work.cancel_work_tag(e);
				return;
			}
			auto soil_id = inventory::find_closest_unclaimed_item_by_category_and_claim_it_immediately<item_topsoil_t>(e.id, pos);
			h.soil_id = soil_id;
			if (soil_id == 0) {
				// No soil available - bail out
				work.cancel_work_tag(e);
				return;
			}
			auto soil_position = inventory::get_item_location(soil_id);
			if (!soil_position) {
				// No soil available - bail out
				work.cancel_work_tag(e);
				return;
			}
			h.current_path.reset();
			h.current_path = find_path(pos, *soil_position);
			if (!h.current_path->success) {
				// No soil available - bail out
				work.cancel_work_tag(e);
				return;
			}
			h.step = ai_tag_work_farm_fixsoil::fix_steps::FETCH_SOIL;
		}

		inline void fetch_soil(entity_t &e, ai_tag_work_farm_fixsoil &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_farm_fixsoil::fix_steps::FIND_SOIL;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_farm_fixsoil::fix_steps::FIND_TARGET;

				inventory_system::pickup_item(h.soil_id, e.id);
				return;
			});
			return;
		}

		inline void find_target(entity_t &e, ai_tag_work_farm_fixsoil &h, ai_tag_my_turn_t &t, position_t &pos) {
			std::map<int, position_t> clear_targets;
			for (const auto &f : farm_designations->farms) {
				if (f.second.state == farm_steps::FIX_SOIL) {
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
					h.step = ai_tag_work_farm_fixsoil::fix_steps::GOTO_TARGET;
					return;
				}
			}
			if (!h.current_path) {
				work.cancel_work_tag(e);
				return;
			}
		}

		inline void goto_target(entity_t &e, ai_tag_work_farm_fixsoil &h, ai_tag_my_turn_t &t, position_t &pos) {
			work.follow_path(h, pos, e, [&h]() {
				// Cancel
				h.current_path.reset();
				h.step = ai_tag_work_farm_fixsoil::fix_steps::FIND_TARGET;
				return;
			}, [&h, &e]() {
				// Success
				h.current_path.reset();
				h.step = ai_tag_work_farm_fixsoil::fix_steps::FIX_TARGET;
				return;
			});
		}

		inline void fix_target(entity_t &e, ai_tag_work_farm_fixsoil &h, ai_tag_my_turn_t &t, position_t &pos) {
			const int idx = mapidx(pos);
			auto farm_finder = farm_designations->farms.find(idx);
			if (farm_finder == farm_designations->farms.end() || farm_finder->second.state != farm_steps::FIX_SOIL) {
				//std::cout << "Bailing out - not relevant anymore!";
				work.cancel_work_tag(e);
				inventory_system::drop_item(h.soil_id, pos.x, pos.y, pos.z);
				return;
			}


			if (work::skill_check_or_damage(e, "Farming", DIFFICULTY_AVERAGE, work, 1, "Farming Accident")) {
				// Transform the tile into soil
				auto topsoil_entity = entity(h.soil_id);
				if (!topsoil_entity) { work.cancel_work_tag(e); return; }
				auto topsoil_item = topsoil_entity->component<item_t>();
				if (!topsoil_item) { work.cancel_work_tag(e); return; }
				region::set_tile_material(idx, topsoil_item->material);

				// Destroy the topsoil item
				inventory::delete_item(h.soil_id);
				h.soil_id = 0;

				// Update the farm
				farm_finder->second.state = farm_steps::PLANT_SEEDS;

				// Update the tile/chunk
				chunks::mark_chunk_dirty_by_tileidx(idx);

				// Become idle - done
				call_home("AI", "Fix Soil");
				work.cancel_work_tag(e);
				h.step = ai_tag_work_farm_fixsoil::fix_steps::FIND_HOE;
			}
		}

		void dispatch(entity_t &e, ai_tag_work_farm_fixsoil &h, ai_tag_my_turn_t &t, position_t &pos) {
			switch (h.step) {
			case ai_tag_work_farm_fixsoil::fix_steps::FIND_HOE : find_hoe(e, h, t, pos); break;
			case ai_tag_work_farm_fixsoil::fix_steps::FETCH_HOE: fetch_hoe(e, h, t, pos); break;
			case ai_tag_work_farm_fixsoil::fix_steps::FIND_SOIL: find_soil(e, h, t, pos); break;
			case ai_tag_work_farm_fixsoil::fix_steps::FETCH_SOIL: fetch_soil(e, h, t, pos); break;
			case ai_tag_work_farm_fixsoil::fix_steps::FIND_TARGET: find_target(e, h, t, pos); break;
			case ai_tag_work_farm_fixsoil::fix_steps::GOTO_TARGET: goto_target(e, h, t, pos); break;
			case ai_tag_work_farm_fixsoil::fix_steps::FIX_TARGET: fix_target(e, h, t, pos); break;
			}
		}

		void run(const double &duration_ms) {
			work.do_work(jobs_board::evaluate_farm_fixing, dispatch, job_tag);
		}
	}
}
