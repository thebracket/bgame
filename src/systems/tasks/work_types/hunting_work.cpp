#include "hunting_work.hpp"
#include "../../ai/path_finding.hpp"
#include "../../ai/inventory_system.hpp"
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
#include "../../ai/distance_map_system.hpp"
#include "../../../messages/messages.hpp"

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

    const int idx = mapidx(pos);
	if (ai.job_type_minor == JM_HUNT_FIND_TARGET) {
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
        const auto distance = huntables_map.distance_map[idx];
        if (distance == 0 || distance == MAX_DIJSTRA_DISTANCE) {
            cancel_action(e, ai, stats, species, pos, name, "No hunting target");
            return;
        }

		// Pick the destination from the Dikstra map
        position_t destination = huntables_map.find_destination(pos);
        move_to(e, pos, destination);
		return;
	}
}

void do_butchering(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
    const int idx = mapidx(pos);

	if (ai.job_type_minor == JM_BUTCHER_FIND_CORPSE) {
        if (butcherables_map.distance_map[idx] > MAX_DIJSTRA_DISTANCE-1) {
            cancel_action(e, ai, stats, species, pos, name, "No butcherable targets");
            return;
        }
        ai.job_type_minor = JM_BUTCHER_GO_TO_CORPSE;
        ai.job_status = "Travel to corpse";
        return;
	}

	if (ai.job_type_minor == JM_BUTCHER_GO_TO_CORPSE) {
        if (butcherables_map.distance_map[idx] > MAX_DIJSTRA_DISTANCE-1) {
            cancel_action(e, ai, stats, species, pos, name, "No butcherable targets");
            return;
        }
        if (butcherables_map.distance_map[idx] == 0) {
            ai.job_type_minor = JM_BUTCHER_COLLECT_CORPSE;
            ai.job_status = "Collecting corpse";
            std::cout << "Collecting corpse\n";
        } else {
            // Keep moving
            position_t destination = butcherables_map.find_destination(pos);
            move_to(e, pos, destination);
        }
		return;
	}

	if (ai.job_type_minor == JM_BUTCHER_COLLECT_CORPSE) {
		ai.job_type_minor = JM_BUTCHER_GO_TO_SHOP;
		ai.job_status = "Carrying corpse to butcher";

        // Find the corpse!
        ai.targeted_hostile = 0;
        each<corpse_harvestable, position_t>([&ai, &pos] (entity_t &E, corpse_harvestable &corpse, position_t &cpos) {
            if (cpos == pos) {
                ai.targeted_hostile = E.id;
            }
        });
        if (ai.targeted_hostile == 0) {
            cancel_action(e, ai, stats, species, pos, name, "Unable to find corpse");
        }

        // Pick up the corpse
        emit_deferred(pickup_item_message{ai.targeted_hostile, e.id});
        ai.current_tool = ai.targeted_hostile; // To ensure that it is dropped on cancellation
        emit_deferred(butcherable_moved_message{});

        // Find the butcher's shop and go there
		position_t butcher_pos;
		each<building_t, position_t>([&butcher_pos] (entity_t &E, building_t &b, position_t &p) {
			if (b.complete == true && b.tag == "butcher") butcher_pos = p;
		});
		ai.current_path = find_path(pos, butcher_pos);
		if (!ai.current_path) {
			cancel_action(e, ai, stats, species, pos, name, "Unable to find butcher shop");
			return;
		}
        ai.job_type_minor = JM_BUTCHER_GO_TO_SHOP;
        ai.job_status = "Carrying corpse to butcher";
		return;
	}

	if (ai.job_type_minor == JM_BUTCHER_GO_TO_SHOP) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_BUTCHER_CHOP;
				change_job_status(ai, name, "Butchering corpse");
                std::cout << "Arrived at butcher shop\n";
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
                std::cout << "Cancelled - can't find route to butcher shop\n";
				cancel_action(e, ai, stats, species, pos, name, "No route to butchers");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_BUTCHER_CHOP) {
		auto corpse = entity(ai.targeted_hostile)->component<corpse_harvestable>();
		const auto organic_idx = get_material_by_tag("organic");
        const auto food_idx = get_material_by_tag("food");

		auto finder = get_creature_def(corpse->creature_tag);
        // Spawn bones
		for (int i=0; i<finder->yield_bone; ++i) {
            auto new_entity = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "bone", food_idx);
            const std::string corpse_type = corpse->creature_tag;
            if (corpse_type != "") {
                auto creature_def = get_creature_def(corpse_type);
                auto item = new_entity->component<item_t>();
                if (creature_def && item) {
                    item->item_name = creature_def->name + std::string(" ") + "Bone";
                }
            }
        }

        // Spawn hide
		for (int i=0; i<finder->yield_hide; ++i) {
            auto new_entity = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "hide", food_idx);
            const std::string corpse_type = corpse->creature_tag;
            if (corpse_type != "") {
                auto creature_def = get_creature_def(corpse_type);
                auto item = new_entity->component<item_t>();
                if (creature_def && item) {
                    item->item_name = creature_def->name + std::string(" ") + "Hide";
                }
            }
        }

        // Spawn meat
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

        // Spawn skulls
		for (int i=0; i<finder->yield_skull; ++i) {
            auto new_entity = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, "skull", food_idx);
            const std::string corpse_type = corpse->creature_tag;
            if (corpse_type != "") {
                auto creature_def = get_creature_def(corpse_type);
                auto item = new_entity->component<item_t>();
                if (creature_def && item) {
                    item->item_name = creature_def->name + std::string(" ") + "Skull";
                }
            }
        }

		delete_entity(ai.targeted_hostile); // Destroy the corpse
        emit_deferred(butcherable_moved_message{});
		become_idle(e, ai, name);
		return;
	}
}
