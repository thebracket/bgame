#include "stdafx.h"
#include "ai_work_farm_clear.hpp"
#include "jobs_board.hpp"
#include "ai_work_template.hpp"
#include "../../../components/farming/designated_farmer.hpp"
#include "../../../global_assets/farming_designations.hpp"
#include "../../../components/ai_tags/ai_tag_work_farm_clear.hpp"
#include "../../../components/claimed_t.hpp"
#include "../../../components/item_tags/item_farming.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../../damage/damage_system.hpp"
#include "../../../render_engine/chunks/chunks.hpp"
#include "../../../raws/materials.hpp"
#include "../../../raws/defs/material_def_t.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../../components/name.hpp"
#include "../../../components/item_tags/item_seed_t.hpp"
#include "../../../raws/plants.hpp"
#include "../../../raws/defs/plant_t.hpp"

#pragma once

namespace systems {
	namespace ai_farm_clear {

		using namespace bengine;
		using namespace jobs_board;
		using namespace distance_map;
		using namespace dijkstra;
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

		static bool first_run = true;

		void run(const double &duration_ms) {
			if (first_run) {
				first_run = false;
				register_job_offer<ai_tag_work_farm_clear>(jobs_board::evaluate_farm_clearance);
			}

			ai_work_template<ai_tag_work_farm_clear> work;
			work.do_ai("Ploughing", [&work](entity_t &e, ai_tag_work_farm_clear &h, ai_tag_my_turn_t &t, position_t &pos) {
				if (h.step == ai_tag_work_farm_clear::clear_steps::FIND_HOE) {
					// Do I already have a pick?
					bool have_tool = false;
					each<item_t, item_carried_t, claimed_t, item_farming_t>([&e, &have_tool](entity_t &E, item_t &i, item_carried_t &ic, claimed_t &claimed, item_farming_t &farm) {
						if (ic.carried_by == e.id) have_tool = true;
					});
					if (have_tool) {
						std::cout << "Skipping get hoe - we already have one!\n";
						h.step = ai_tag_work_farm_clear::clear_steps::FIND_TARGET;
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
								h.step = ai_tag_work_farm_clear::clear_steps::FETCH_HOE;
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
				else if (h.step == ai_tag_work_farm_clear::clear_steps::FETCH_HOE) {
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
					return;
				}
				else if (h.step == ai_tag_work_farm_clear::clear_steps::FIND_TARGET) {
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
					return;
				}
				else if (h.step == ai_tag_work_farm_clear::clear_steps::GOTO_TARGET) {
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
					return;
				}
				else if (h.step == ai_tag_work_farm_clear::clear_steps::CLEAR_TARGET) {
					const int idx = mapidx(pos);
					auto stats = e.component<game_stats_t>();
					if (!stats) {
						std::cout << "Cancelled because of missing stats.\n";
						work.cancel_work_tag(e);
						return;
					}

					auto farm_finder = farm_designations->farms.find(idx);
					if (farm_finder == farm_designations->farms.end() || farm_finder->second.state != farm_steps::CLEAR) {
						std::cout << "Bailing out - not relevant anymore!";
						work.cancel_work_tag(e);
						return;
					}

					auto skill_check = skill_roll(e.id, *stats, rng, "Farming", DIFFICULTY_EASY);
					if (skill_check >= SUCCESS) {
						// Harvest anything here
						if (veg_type(idx) < 0) {
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
						if (tm && tm->spawn_type != soil) {
							farm_finder->second.state = farm_steps::FIX_SOIL;
						}
						else {
							farm_finder->second.state = farm_steps::PLANT_SEEDS;
						}

						// Become idle - done
						call_home("AI", "Plough");
						work.cancel_work_tag(e);
						h.step = ai_tag_work_farm_clear::clear_steps::FIND_HOE;
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