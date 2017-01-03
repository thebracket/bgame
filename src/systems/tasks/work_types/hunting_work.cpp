#include "hunting_work.hpp"
#include "../../path_finding.hpp"
#include "../../inventory_system.hpp"
#include "../../ai/wildlife_population_system.hpp"
#include "../../damage/weapons_helpers.hpp"
#include "../idle_mode.hpp"
#include "../settler_glyph.hpp"
#include "../settler_job_status.hpp"
#include "../settler_drop_tool.hpp"
#include "../settler_cancel_action.hpp"
#include "../pathfinding.hpp"
#include "../initiative.hpp"
#include "../../../components/corpse_harvestable.hpp"
#include "../settler_move_to.hpp"
#include "../../../raws/creatures.hpp"
#include "../../distance_map_system.hpp"

#include <iostream>
#include <map>

using namespace rltk;
using tasks::become_idle;
using tasks::change_settler_glyph;
using tasks::change_job_status;
using tasks::drop_current_tool;
using tasks::cancel_action;
using tasks::follow_path;
using tasks::follow_result_t;
using tasks::calculate_initiative;

void do_hunting(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	auto ranged_status = has_ranged_weapon(e);
	bool has_ammo = has_appropriate_ammo(e, ranged_status.second, pos);
	if (!ranged_status.first || !has_ammo) {
		cancel_action(e, ai, stats, species, pos, name, "Out of ammo");
		return;
	}
	
	if (ai.job_type_minor == JM_HUNT_FIND_TARGET) {
        const int idx = mapidx(pos);
        const auto distance = huntables_map.distance_map[idx];
        if (distance == 0 || distance == MAX_DIJSTRA_DISTANCE) {
            cancel_action(e, ai, stats, species, pos, name, "No hunting target");
            return;
        }

        ai.job_type_minor = JM_HUNT;
        ai.job_status = "Hunting";
        return;
	}

	if (ai.job_type_minor == JM_HUNT) {
		// Pick the destination from the Dikstra map
        position_t destination = huntables_map.find_destination(pos);
        move_to(e, pos, destination);
		return;
	}
}

void do_butchering(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_BUTCHER_FIND_CORPSE) {
		std::map<int, std::pair<std::size_t, position_t>> corpse_map;
		each<corpse_harvestable, position_t>([&corpse_map, &pos] (entity_t &E, corpse_harvestable &corpse, position_t &cpos) {
			if (!corpse.claimed) {
				corpse_map[distance3d_squared(pos.x, pos.y, pos.z, cpos.x, cpos.y, cpos.z)] = std::make_pair(E.id, cpos);
			}
		});

		ai.current_path.reset();
		auto it = corpse_map.begin();
		while (it != corpse_map.end() && !ai.current_path) {
			ai.current_path = find_path(pos, it->second.second);
			if (!ai.current_path) ++it;
		}
		if (!ai.current_path) {
			cancel_action(e, ai, stats, species, pos, name, "No butcherable targets");
			return;
		} else {
			ai.job_type_minor = JM_BUTCHER_GO_TO_CORPSE;
			ai.job_status = "Travel to corpse";
			entity(it->second.first)->component<corpse_harvestable>()->claimed = true;
			ai.targeted_hostile = it->second.first;
			return;
		}		
	}

	if (ai.job_type_minor == JM_BUTCHER_GO_TO_CORPSE) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_BUTCHER_COLLECT_CORPSE;
				change_job_status(ai, name, "Collecting corpse");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				cancel_action(e, ai, stats, species, pos, name, "No route to corpse");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_BUTCHER_COLLECT_CORPSE) {
		ai.job_type_minor = JM_BUTCHER_GO_TO_SHOP;
		ai.job_status = "Carrying corpse to butcher";
		position_t butcher_pos;
		each<building_t, position_t>([&butcher_pos] (entity_t &E, building_t &b, position_t &p) {
			if (b.complete == true && b.tag == "butcher") butcher_pos = p;
		});
		ai.current_path = find_path(pos, butcher_pos);
		if (!ai.current_path) {
			cancel_action(e, ai, stats, species, pos, name, "Unable to find butcher shop");
			return;
		}
		return;
	}

	if (ai.job_type_minor == JM_BUTCHER_GO_TO_SHOP) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_BUTCHER_CHOP;
				change_job_status(ai, name, "Butchering corpse");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				cancel_action(e, ai, stats, species, pos, name, "No route to butchers");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_BUTCHER_CHOP) {
		auto corpse = entity(ai.targeted_hostile)->component<corpse_harvestable>();
		const auto organic_idx = get_material_by_tag("organic").get();
        const auto food_idx = get_material_by_tag("food").get();

		auto finder = get_creature_def(corpse->creature_tag);
		for (int i=0; i<finder->yield_bone; ++i) spawn_item_on_ground(pos.x, pos.y, pos.z, "bone", organic_idx);
		for (int i=0; i<finder->yield_hide; ++i) spawn_item_on_ground(pos.x, pos.y, pos.z, "hide", organic_idx);
		for (int i=0; i<finder->yield_meat; ++i) {
            auto new_entity = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "meat", food_idx);
            const std::string corpse_type = corpse->creature_tag;
            if (corpse_type != "") {
                auto creature_def = get_creature_def(corpse_type);
                auto item = new_entity->component<item_t>();
                if (creature_def && item) {
                    item->item_name = creature_def->name + std::string(" ") + "Meat";
                }
            }
        }
		for (int i=0; i<finder->yield_skull; ++i) spawn_item_on_ground(pos.x, pos.y, pos.z, "skull", organic_idx);

		delete_entity(ai.targeted_hostile); // Destroy the corpse
		become_idle(e, ai, name);
		return;
	}
}
