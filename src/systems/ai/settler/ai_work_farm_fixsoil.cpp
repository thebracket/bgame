#include "jobs_board.hpp"
#include "ai_work_template.hpp"
#include "ai_work_farm_fixsoil.hpp"
#include "../../../components/ai_tags/ai_tag_work_farm_fixsoil.hpp"
#include "../../../components/farming/designated_farmer.hpp"
#include "../../../global_assets/farming_designations.hpp"
#include "../../../components/claimed_t.hpp"
#include "../../../components/item_tags/item_farming.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../../damage/damage_system.hpp"
#include "../../../render_engine/chunks/chunks.hpp"
#include "../../../components/item_tags/item_topsoil_t.hpp"
#include "../../../bengine/telemetry.hpp"

namespace systems {
	namespace ai_farm_fixsoil {
		using namespace bengine;
		using namespace jobs_board;
		using namespace distance_map;
		using namespace dijkstra;
		using namespace region;

		namespace jobs_board {
			void evaluate_farm_clearance(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
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

		static bool first_run = true;

		void run(const double &duration_ms) {
			if (first_run) {
				first_run = false;
				register_job_offer<ai_tag_work_farm_fixsoil>(jobs_board::evaluate_farm_clearance);
			}

			ai_work_template<ai_tag_work_farm_fixsoil> work;
			work.do_ai("Fixing Soil", [&work](entity_t &e, ai_tag_work_farm_fixsoil &h, ai_tag_my_turn_t &t, position_t &pos) {
				if (h.step == ai_tag_work_farm_fixsoil::fix_steps::FIND_HOE) {
					// Do I already have a pick?
					bool have_tool = false;
					each<item_t, item_carried_t, claimed_t, item_farming_t>([&e, &have_tool](entity_t &E, item_t &i, item_carried_t &ic, claimed_t &claimed, item_farming_t &farm) {
						if (ic.carried_by == e.id) have_tool = true;
					});
					if (have_tool) {
						std::cout << "Skipping get hoe - we already have one!\n";
						h.step = ai_tag_work_farm_fixsoil::fix_steps::FIND_SOIL;
						return;
					}
					else {
						// We need to fetch the pick
						std::size_t tool_id = -1;
						each<item_t, claimed_t, item_farming_t>([&e, &tool_id](entity_t &E, item_t &i, claimed_t &claimed, item_farming_t &farm) {
							if (claimed.claimed_by == e.id) tool_id = E.id;
						});
						if (tool_id == -1) {
							std::cout << "Unable to find claimed tool. Bug!\n";
							work.cancel_work_tag(e);
							return;
						}

						h.tool_id = tool_id;
						auto tool_pos = inventory::get_item_location(tool_id);
						if (tool_pos) {
							h.current_path = find_path(pos, *tool_pos);
							if (h.current_path->success) {
								h.step = ai_tag_work_farm_fixsoil::fix_steps::FETCH_HOE;
							}
							else {
								work.cancel_work_tag(e);
								return;
							}
						}
						else {
							work.cancel_work_tag(e);
							return;
						}
					}
				}
				else if (h.step == ai_tag_work_farm_fixsoil::fix_steps::FETCH_HOE) {
					work.follow_path(h, pos, e, [&h]() {
						// Cancel
						h.current_path.reset();
						h.step = ai_tag_work_farm_fixsoil::fix_steps::FIND_HOE;
						return;
					}, [&h, &e]() {
						// Success
						h.current_path.reset();
						h.step = ai_tag_work_farm_fixsoil::fix_steps::FIND_TARGET;

						inventory_system::pickup_item(h.tool_id, e.id);
						return;
					});
					return;
				}
				else if (h.step == ai_tag_work_farm_fixsoil::fix_steps::FIND_SOIL) {
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
					return;
				}
				else if (h.step == ai_tag_work_farm_fixsoil::fix_steps::FETCH_SOIL) {
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
				else if (h.step == ai_tag_work_farm_fixsoil::fix_steps::FIND_TARGET) {
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
					return;
				}
				else if (h.step == ai_tag_work_farm_fixsoil::fix_steps::GOTO_TARGET) {
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
					return;
				}
				else if (h.step == ai_tag_work_farm_fixsoil::fix_steps::FIX_TARGET) {
					const int idx = mapidx(pos);
					auto stats = e.component<game_stats_t>();
					if (!stats) {
						std::cout << "Cancelled because of missing stats.\n";
						work.cancel_work_tag(e);
						inventory_system::drop_item(h.soil_id, pos.x, pos.y, pos.z);
						return;
					}

					auto farm_finder = farm_designations->farms.find(idx);
					if (farm_finder == farm_designations->farms.end() || farm_finder->second.state != farm_steps::FIX_SOIL) {
						std::cout << "Bailing out - not relevant anymore!";
						work.cancel_work_tag(e);
						inventory_system::drop_item(h.soil_id, pos.x, pos.y, pos.z);
						return;
					}

					auto skill_check = skill_roll(e.id, *stats, rng, "Farming", DIFFICULTY_AVERAGE);
					if (skill_check >= SUCCESS) {

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
						return;
					}
					else {
						if (skill_check == CRITICAL_FAIL) damage_system::inflict_damage(damage_system::inflict_damage_message{ e.id, 1, "Farming Accident" });
						return;
					}
				}

			});
		}
	}
}