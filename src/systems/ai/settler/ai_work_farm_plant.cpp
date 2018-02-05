#include "stdafx.h"
#include "jobs_board.hpp"
#include "ai_work_template.hpp"
#include "../../../components/ai_tags/ai_tag_work_farm_plant.hpp"
#include "../../../components/farming/designated_farmer.hpp"
#include "../../../global_assets/farming_designations.hpp"
#include "../../../components/claimed_t.hpp"
#include "../../../components/item_tags/item_farming.hpp"
#include "../../helpers/inventory_assistant.hpp"
#include "../../damage/damage_system.hpp"
#include "../../../render_engine/chunks/chunks.hpp"
#include "ai_work_farm_plant.hpp"
#include "../../../components/item_tags/item_seed_t.hpp"
#include "../../../bengine/telemetry.hpp"
#include "../../../raws/plants.hpp"
#include "../../../raws/defs/plant_t.hpp"

#pragma once

namespace systems {
	namespace ai_farm_plant {
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
					if (farm.second.state == farm_steps::PLANT_SEEDS) {
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
				register_job_offer<ai_tag_work_farm_plant>(jobs_board::evaluate_farm_clearance);
			}

			ai_work_template<ai_tag_work_farm_plant> work;
			work.do_ai([&work](entity_t &e, ai_tag_work_farm_plant &h, ai_tag_my_turn_t &t, position_t &pos) {
				work.set_status(e, "Planting");
				if (h.step == ai_tag_work_farm_plant::plant_steps::FIND_HOE) {
					// Do I already have a pick?
					bool have_tool = false;
					each<item_t, item_carried_t, claimed_t, item_farming_t>([&e, &have_tool](entity_t &E, item_t &i, item_carried_t &ic, claimed_t &claimed, item_farming_t &farm) {
						if (ic.carried_by == e.id) have_tool = true;
					});
					if (have_tool) {
						std::cout << "Skipping get hoe - we already have one!\n";
						h.step = ai_tag_work_farm_plant::plant_steps::FIND_SEED;
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
								h.step = ai_tag_work_farm_plant::plant_steps::FETCH_HOE;
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
				else if (h.step == ai_tag_work_farm_plant::plant_steps::FETCH_HOE) {
					work.follow_path(h, pos, e, [&h]() {
						// Cancel
						h.current_path.reset();
						h.step = ai_tag_work_farm_plant::plant_steps::FIND_HOE;
						return;
					}, [&h, &e]() {
						// Success
						h.current_path.reset();
						h.step = ai_tag_work_farm_plant::plant_steps::FIND_TARGET;

						inventory_system::pickup_item(h.tool_id, e.id);
						return;
					});
					return;
				}
				else if (h.step == ai_tag_work_farm_plant::plant_steps::FIND_SEED) {
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
						std::cout << seed_type << "\n";
						std::size_t seed_id = 0;
						each<claimed_t, item_seed_t>([&seed_type, &seed_id](entity_t &se, claimed_t &claim, item_seed_t &seed) {
							std::cout << seed.grows_into << "\n";
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
					return;
				}
				else if (h.step == ai_tag_work_farm_plant::plant_steps::FETCH_SEED) {
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
					return;
				}
				else if (h.step == ai_tag_work_farm_plant::plant_steps::FIND_TARGET) {
					h.current_path.reset();
					h.current_path = find_path(pos, h.farm_position);
					if (!h.current_path->success) {
						work.cancel_work_tag(e);
						return;
					}
					h.step = ai_tag_work_farm_plant::plant_steps::FETCH_TARGET;
				}
				else if (h.step == ai_tag_work_farm_plant::plant_steps::FETCH_TARGET) {
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
					return;
				}
				else if (h.step == ai_tag_work_farm_plant::plant_steps::PLANT) {
					const int idx = mapidx(pos);
					auto stats = e.component<game_stats_t>();
					if (!stats) {
						std::cout << "Cancelled because of missing stats.\n";
						inventory_system::drop_item(h.seed_id, pos.x, pos.y, pos.z);
						work.cancel_work_tag(e);
						return;
					}

					auto farm_finder = farm_designations->farms.find(idx);
					if (farm_finder == farm_designations->farms.end() || farm_finder->second.state != farm_steps::PLANT_SEEDS) {
						std::cout << "Bailing out - not relevant anymore!";
						inventory_system::drop_item(h.seed_id, pos.x, pos.y, pos.z);
						work.cancel_work_tag(e);
						return;
					}

					auto skill_check = skill_roll(e.id, *stats, rng, "Farming", DIFFICULTY_AVERAGE);
					if (skill_check >= SUCCESS) {
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