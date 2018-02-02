#include "stdafx.h"
#include "ai_sleep_time.hpp"
#include "jobs_board.hpp"
#include "ai_work_template.hpp"
#include "../../../components/position.hpp"
#include "../../../components/ai_tags/ai_tag_work_harvest.hpp"
#include "../../../global_assets/game_designations.hpp"
#include "../distance_map_system.hpp"
#include "../../../planet/region/region.hpp"
#include "../../../raws/plants.hpp"
#include "../../../raws/defs/plant_t.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../../components/item_tags/item_food_t.hpp"
#include "../../../components/item_tags/item_spice_t.hpp"
#include "../../../components/name.hpp"
#include "../../../components/farming/designated_farmer.hpp"
#include "../../../components/claimed_t.hpp"
#include "../../../components/item_tags/item_farming.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../../../bengine/geometry.hpp"
#include "../../../render_engine/chunks/chunks.hpp"
#include "../../damage/damage_system.hpp"

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
				if (designations->harvest.empty()) return; // Nothing to harvest

				board.insert(std::make_pair((int)bengine::distance3d(pos.x, pos.y, pos.z, designations->harvest.begin()->second.x, designations->harvest.begin()->second.y, designations->harvest.begin()->second.z), jt));
			}
		}

		static bool first_run = true;

		void run(const double &duration_ms) {
			if (first_run) {
				first_run = false;
				register_job_offer<ai_tag_work_harvest>(jobs_board::evaluate_harvest);
			}

			ai_work_template<ai_tag_work_harvest> work;
			work.do_ai([&work](entity_t &e, ai_tag_work_harvest &h, ai_tag_my_turn_t &t, position_t &pos) {
				work.set_status(e, "Harvesting");
				if (h.step == ai_tag_work_harvest::harvest_steps::FIND_HOE) {
					// Do I already have a pick?
					bool have_tool = false;
					each<item_t, item_carried_t, claimed_t, item_farming_t>([&e, &have_tool](entity_t &E, item_t &i, item_carried_t &ic, claimed_t &claimed, item_farming_t &farm) {
						if (ic.carried_by == e.id) have_tool = true;
					});
					if (have_tool) {
						std::cout << "Skipping get pick - we already have one!\n";
						h.step = ai_tag_work_harvest::harvest_steps::FIND_HARVEST;
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
								h.step = ai_tag_work_harvest::harvest_steps::FETCH_HOE;
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
				else if (h.step == ai_tag_work_harvest::harvest_steps::FETCH_HOE) {
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
					return;
				}
				if (h.step == ai_tag_work_harvest::harvest_steps::FIND_HARVEST) {
					std::map<int, position_t> harvest_targets;
					for (const auto ht : designations->harvest) {
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
					return;
				}
				else if (h.step == ai_tag_work_harvest::harvest_steps::GOTO_HARVEST) {
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
				else if (h.step == ai_tag_work_harvest::harvest_steps::DO_HARVEST) {
					const int idx = mapidx(pos);
					designations->harvest.erase(std::remove_if(
						designations->harvest.begin(),
						designations->harvest.end(),
						[&idx](std::pair<bool, position_t> p) { return idx == mapidx(p.second); }
					),
						designations->harvest.end());

					auto stats = e.component<game_stats_t>();
					if (!stats) {
						std::cout << "Cancelled because of missing stats.\n";
						work.cancel_work_tag(e);
						return;
					}

					auto skill_check = skill_roll(e.id, *stats, rng, "Farming", DIFFICULTY_EASY);
					if (skill_check >= SUCCESS) {

						// Create the harvesting result
						if (veg_type(idx) == 0) {
							work.cancel_work_tag(e);
							return;
						}
						const auto plant = get_plant_def(veg_type(idx));
						const std::string result = plant->provides[veg_lifecycle(idx)];
						if (result != "none") {
							std::string mat_type = "organic";
							auto item_finder = get_item_def(result);
							if (item_finder != nullptr) {
								if (item_finder->categories.test(ITEM_FOOD)) mat_type = "food";
								if (item_finder->categories.test(ITEM_SPICE)) mat_type = "spice";
							}
							std::string cname = "";
							auto name = e.component<name_t>();
							if (name) cname = name->first_name + std::string(" ") + name->last_name;
							auto item = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, result, get_material_by_tag(mat_type), 3, 100, e.id, cname);
							item->component<item_t>()->item_name = plant->name;
						}

						// Knock tile back to germination
						set_veg_lifecycle(idx, 0);
						set_veg_ticker(idx, 0);
						chunks::mark_chunk_dirty_by_tileidx(idx);

						// Become idle - done
						call_home("AI", "Harvest", result);
						work.cancel_work_tag(e);
						h.step = ai_tag_work_harvest::harvest_steps::FIND_HOE;
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