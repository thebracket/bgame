#include "settler_ai_system.hpp"
#include "../messages/messages.hpp"
#include "../main/game_globals.hpp"
#include "path_finding.hpp"
#include "mining_system.hpp"
#include "inventory_system.hpp"
#include "workflow_system.hpp"
#include "wildlife_population_system.hpp"
#include "weapons_helpers.hpp"
#include "../components/item_carried.hpp"
#include "tasks/idle_mode.hpp"
#include "tasks/settler_glyph.hpp"
#include "tasks/settler_job_status.hpp"
#include "tasks/settler_drop_tool.hpp"
#include "tasks/settler_cancel_action.hpp"
#include "tasks/pathfinding.hpp"
#include "tasks/initiative.hpp"
#include "../messages/log_message.hpp"
#include "../components/logger.hpp"
#include "../components/health.hpp"
#include "../components/renderable.hpp"
#include "../components/corpse_harvestable.hpp"
#include "../components/construct_provides_sleep.hpp"
#include "../components/viewshed.hpp"
#include "../components/smoke_emitter.hpp"
#include "../components/grazer_ai.hpp"
#include "../components/sentient_ai.hpp"
#include "../components/lightsource.hpp"
#include "../components/falling.hpp"
#include "tasks/threat_scanner.hpp"
#include "tasks/world_queries.hpp"

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

void settler_ai_system::update(const double duration_ms) {
	std::queue<tick_message> * ticks = mbox<tick_message>();
    while (!ticks->empty()) {
        ticks->pop();

		bool found_settler = false;
		each<settler_ai_t, game_stats_t, species_t, position_t, name_t, health_t, viewshed_t>([this, &found_settler] (entity_t &entity, settler_ai_t &ai, game_stats_t &stats, 
			species_t &species, position_t &pos, name_t &name, health_t &health, viewshed_t &view) 
		{
			found_settler = true;
			int initiative_penalty = 0;

			if (entity.component<falling_t>()) {
				emit_deferred(LOG{}.settler_name(entity.id)->text(" says 'arrrghh'"));
				return;
			}

			if (ai.initiative < 1) {
				if (game_master_mode == ROGUE && entity.id == selected_settler) return; // We handle this in the rogue system
				if (health.unconscious) return; // Do nothing - they passed out!

				const int shift_id = ai.shift_id;
				const int hour_of_day = calendar->hour;
				const shift_type_t current_schedule = calendar->defined_shifts[shift_id].hours[hour_of_day];

				if (tasks::is_stuck_or_invalid(pos)) {
					emit_deferred(log_message{LOG{}.text("Warning - settler is stuck; activating emergency teleport to bed!")->chars});
					bool done = false;
					each<position_t, construct_provides_sleep_t>([this,&entity,&pos,&done] (entity_t &E, position_t &P, construct_provides_sleep_t &S) {
						if (!done) {
							move_to(entity, pos, P);
							done = true;
							// This should use power
						}
					});
				}

				// Do we have any hostiles to worry about?
				auto hostile = tasks::can_see_hostile(entity, pos, view, [&ai] (entity_t &other) {
					if (ai.job_type_minor == JM_SLEEP) return false; // We don't spot anyone when sleeping
					if (other.component<grazer_ai>() && designations->standing_order_wildlife_treatment != standing_orders::SO_WILDLIFE_IGNORE) return true;
					auto sentient = other.component<sentient_ai>();
					if (sentient && sentient->hostile) return true;
					return false;
				});

				if (hostile.terrified) {
					// Run away! Eventually, we want the option for combat here based on morale. Also, when hunting
					// is implemented it's a good idea not to run away from your target.
					const int range = shooting_range(entity, pos);
					if (hostile.terror_distance < 1.5F) {
						// Hit it with melee weapon
						emit_deferred(settler_attack_message{entity.id, hostile.closest_fear});
						initiative_penalty += get_weapon_initiative_penalty(get_melee_id(entity));
					} else if (range != -1 && range < hostile.terror_distance) {
						// Shoot it
						emit_deferred(settler_ranged_attack_message{entity.id, hostile.closest_fear});
						initiative_penalty += get_weapon_initiative_penalty(get_ranged_and_ammo_id(entity).first);
					} else {
						emit_deferred(entity_wants_to_flee_message{entity.id, hostile.closest_fear});
					}
				} else if (ai.job_type_major == JOB_MINE || ai.job_type_major == JOB_CHOP || ai.job_type_major == JOB_CONST
					|| ai.job_type_major == JOB_REACTION || ai.job_type_major == JOB_BUTCHER || ai.job_type_major == JOB_DECONSTRUCT) {
						// If we have a job to do - keep doing it
						this->do_work_time(entity, ai, stats, species, pos, name);
				} else {
					switch (current_schedule) {
						case SLEEP_SHIFT : this->do_sleep_time(entity, ai, stats, species, pos, name); break;
						//case LEISURE_SHIFT : this->do_leisure_time(entity, ai, stats, species, pos, name); break;
						// Temporary - everyone works
						case LEISURE_SHIFT : this->do_work_time(entity, ai, stats, species, pos, name); break;
						case WORK_SHIFT : this->do_work_time(entity, ai, stats, species, pos, name); break;
					}
				}
				
				calculate_initiative(ai, stats, initiative_penalty);
			} else {
				--ai.initiative;
			}
		});

		if (!found_settler) {
			emit_deferred(game_over_message{1});
		}
	}
}

void settler_ai_system::wander_randomly(entity_t &entity, position_t &original) {
	// Never wander if it is prohibited
	if (designations->standing_order_idle_move == standing_orders::SO_IDLE_STATIC) return;

	// Don't wander far from Cordex if that is prohibited
	if (designations->standing_order_idle_move == standing_orders::SO_IDLE_WANDER_CLOSE) {
		const float distance = distance3d(original.x, original.y, original.z, REGION_WIDTH/2, REGION_HEIGHT/2, REGION_DEPTH/2);
		if (distance > 15.0F) return;
	}

	auto render = entity.component<renderable_t>();
	render->foreground = rltk::colors::YELLOW;
	render->glyph = 1;
	emit_deferred(entity_wants_to_move_randomly_message{entity.id});
}

void settler_ai_system::configure() {
	subscribe_mbox<tick_message>();
	system_name = "Settler AI";
}

void settler_ai_system::move_to(entity_t &e, position_t &pos, position_t &destination) {
	emit_deferred(entity_wants_to_move_message{e.id, destination});
}

void settler_ai_system::do_sleep_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.current_tool != 0) drop_current_tool(entity, ai, pos);

	if (ai.job_type_major != JOB_IDLE && ai.job_type_major != JOB_SLEEP) {
		cancel_action(entity, ai, stats, species, pos, name, "Bed time!");
		return;
	}
	if (ai.job_type_major == JOB_IDLE) {
		ai.job_type_major = JOB_SLEEP;
		ai.job_type_minor = JM_FIND_BED;
		change_job_status(ai, name, "Looking for a bed");
		return;
	}
	if (ai.job_type_major != JOB_SLEEP) throw std::runtime_error("Sleep mode, but shouldn't have made it to here.\n");

	if (ai.job_type_minor == JM_FIND_BED) {
		// Find nearest unclaimed bed
		std::map<float, std::tuple<construct_provides_sleep_t,position_t,std::size_t>> bed_candidates;
		each<construct_provides_sleep_t, position_t>([&bed_candidates, &pos] (entity_t &e, construct_provides_sleep_t &bed, position_t &bed_pos) {
			if (!bed.claimed) {
				if (find_path(pos, bed_pos)->success) {
					bed_candidates[distance3d(pos.x, pos.y, pos.z, bed_pos.x, bed_pos.y, bed_pos.z)] = std::make_tuple(bed, bed_pos, e.id);
				}
			}
		});
		if (bed_candidates.empty()) {
			emit_deferred(log_message{LOG{}.settler_name(entity.id)->text(" cannot find a bed, and is sleeping rough.")->chars});
			ai.job_type_minor = JM_SLEEP;
			ai.target_id = 0;
			return;
		}

		// Claim it
		ai.target_x = std::get<1>(bed_candidates.begin()->second).x;
		ai.target_y = std::get<1>(bed_candidates.begin()->second).y;
		ai.target_z = std::get<1>(bed_candidates.begin()->second).z;
		ai.target_id = std::get<2>(bed_candidates.begin()->second);
		rltk::entity(ai.target_id)->component<construct_provides_sleep_t>()->claimed = true;

		// Set the path
		ai.current_path = find_path(pos, position_t{ai.target_x, ai.target_y, ai.target_z});
		if (!ai.current_path->success) {
			std::cout << "Warning: no path to bed found.\n";
			ai.current_path.reset();
			rltk::entity(ai.target_id)->component<construct_provides_sleep_t>()->claimed = false;
			return;
		}
		ai.job_type_minor = JM_GO_TO_BED;
		change_job_status(ai, name, "Going to bed");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_BED) {
		tasks::try_path(entity, ai, pos,
			[] () {}, // Do nothing for success
			[&ai, &name] () {
				ai.job_type_minor = JM_SLEEP;
				change_job_status(ai, name, "Sleeping", true);
			}, // Arrived
			[&ai, &name, &entity] () {
				ai.job_type_minor = JM_FIND_BED;
				change_job_status(ai, name, "Looking for a bed");
				emit_deferred(log_message{LOG{}.settler_name(entity.id)->text(" cannot find a bed, and is sleeping rough.")->chars});
				rltk::entity(ai.target_id)->component<construct_provides_sleep_t>()->claimed = false;
				ai.job_type_minor = JM_SLEEP;
			} // fail
		);
		return;
	}

	if (ai.job_type_minor == JM_SLEEP) {
		auto render = entity.component<renderable_t>();
		render->foreground = rltk::colors::BLUE;

		if (rng.roll_dice(1,6) < 3) {
			render->glyph = 1;
		} else {
			render->glyph = 'Z';
		}

		auto health = entity.component<health_t>();
		if (health && health->current_hitpoints < health->max_hitpoints && rng.roll_dice(1,20) + stat_modifier(stats.constitution) > 12) {
			++health->current_hitpoints;
		}

		emit_deferred(renderables_changed_message{});
	}
}

void settler_ai_system::do_leisure_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.current_tool != 0) drop_current_tool(entity, ai, pos);
	if (ai.job_type_major == JOB_SLEEP) {
		cancel_action(entity, ai, stats, species, pos, name, "Time to wake up");
		return;
	}
	wander_randomly(entity, pos);
	if (ai.job_status != "Idle") ai.job_status = "Idle";
}

inline rltk::color_t get_task_color(const std::string &skill) {
	if (skill == "Masonry") {
		return rltk::colors::LightGrey;
	} else if (skill == "Carpentry") {
		return rltk::colors::BurlyWood;
	} else if (skill == "Furnace Operation") {
		return rltk::colors::Orange;
	} else if (skill == "Glassmaking") {
		return rltk::colors::CYAN;
	} else if (skill == "Metalworking") {
		return rltk::colors::BRASS;
	}
	return rltk::colors::Grey;
}

void settler_ai_system::do_work_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_major == JOB_SLEEP) {
		cancel_action(entity, ai, stats, species, pos, name, "Time to wake up");
		return;
	}
	if (ai.job_type_major == JOB_IDLE) {
		// Find something to do!
		const auto idx = mapidx(pos.x, pos.y, pos.z);
		
		if (ai.permitted_work[JOB_GUARDING] && !designations->guard_points.empty() && shooting_range(entity, pos)>0) {
			for (auto &g : designations->guard_points) {
				if (!g.first) {
					g.first = true;
					ai.job_type_major = JOB_GUARD;
					ai.job_type_minor = JM_FIND_GUARDPOST;
					ai.target_x = g.second.x;
					ai.target_y = g.second.y;
					ai.target_z = g.second.z;
					change_job_status(ai, name, "starting guard duty.", true);
					return;
				}
			}
		}

		if (ai.permitted_work[JOB_MINING] && mining_map[idx]<250 && is_item_category_available(TOOL_DIGGING)) {
			change_settler_glyph(entity, vchar{1, rltk::colors::WHITE, rltk::colors::BLACK});
			ai.job_type_major = JOB_MINE;
			ai.job_type_minor = JM_FIND_PICK;
			change_job_status(ai, name, "doing some mining.", true);
			return;
		}
		if (ai.permitted_work[JOB_CHOPPING] && designations->chopping.size() > 0 && is_item_category_available(TOOL_CHOPPING)) {
			change_settler_glyph(entity, vchar{1, rltk::colors::Brown, rltk::colors::BLACK});
			ai.job_type_major = JOB_CHOP;
			ai.job_type_minor = JM_FIND_AXE;
			change_job_status(ai, name, "chopping down a tree.", true);
			return;
		}
		if (ai.permitted_work[JOB_CONSTRUCTION] && designations->buildings.size() > 0) {
			ai.building_target.reset();

			ai.building_target = (designations->buildings.back());
			designations->buildings.pop_back();

			if (ai.building_target) {
				change_settler_glyph(entity, vchar{1, rltk::colors::Pink, rltk::colors::BLACK});
				ai.job_type_major = JOB_CONST;
				ai.job_type_minor = JM_SELECT_COMPONENT;
				change_job_status(ai, name, "performing construction.", true);
			}
			return;
		}
		if (ai.permitted_work[JOB_CONSTRUCTION] && designations->deconstructions.size() > 0) {
			unbuild_t building = designations->deconstructions.back();
			designations->deconstructions.pop_back();
			if (building.is_building) {
				ai.target_id = building.building_id;
				change_settler_glyph(entity, vchar{1, rltk::colors::Pink, rltk::colors::BLACK});
				ai.job_type_major = JOB_DECONSTRUCT;
				ai.job_type_minor = JM_FIND_DECONSTRUCT;
				change_job_status(ai, name, "performing demolition.", true);
			} else {
				std::cout << building.building_id << "\n";
				ai.target_id = building.building_id;
				change_settler_glyph(entity, vchar{1, rltk::colors::RED, rltk::colors::BLACK});
				ai.job_type_major = JOB_DEMOLISH;
				ai.job_type_minor = JM_FIND_DEMOLISH;
				change_job_status(ai, name, "performing structural demolition.", true);
			}
			return;
		}

		// Look for a queued order to perform
		if (!designations->build_orders.empty()) {
			boost::optional<reaction_task_t> autojob = find_queued_reaction_task(ai);

			if (autojob) {
				auto finder = reaction_defs.find(autojob.get().reaction_tag);
				change_settler_glyph(entity, vchar{1, get_task_color(finder->second.skill), rltk::colors::BLACK});
				ai.job_type_major = JOB_REACTION;
				ai.job_type_minor = JM_SELECT_INPUT;
				change_job_status(ai, name, autojob.get().job_name, true);
				ai.reaction_target = autojob;
				return;
			}
		}

		// Look for an automatic reaction to perform
		boost::optional<reaction_task_t> autojob = find_automatic_reaction_task(ai);
		if (autojob) {
			auto finder = reaction_defs.find(autojob.get().reaction_tag);
			change_settler_glyph(entity, vchar{1, get_task_color(finder->second.skill), rltk::colors::BLACK});
			ai.job_type_major = JOB_REACTION;
			ai.job_type_minor = JM_SELECT_INPUT;
			change_job_status(ai, name, autojob.get().job_name, true);
			ai.reaction_target = autojob;
			return;
		}

		// If we don't have a ranged weapon, and one is available, equip it
		std::pair<bool, std::string> ranged_status = has_ranged_weapon(entity);
		if (is_item_category_available(WEAPON_RANGED) && ranged_status.first==false) {
			change_settler_glyph(entity, vchar{1, rltk::colors::WHITE, rltk::colors::BLACK});
			ai.job_type_major = JOB_EQUIP_RANGED;
			ai.job_type_minor = JM_FIND_RANGED_WEAPON;
			change_job_status(ai, name, "Finding a ranged weapon.", false);
			return;
		}

		// Likewise, search for ammo if available
		bool has_ammo = has_appropriate_ammo(entity, ranged_status.second, pos);
		if (designations->standing_order_upgrade > standing_orders::SO_UPGRADE_NEVER && ranged_status.first && !has_ammo && is_ammo_available(ranged_status.second)) {
			change_settler_glyph(entity, vchar{1, rltk::colors::WHITE, rltk::colors::BLACK});
			ai.job_type_major = JOB_EQUIP_AMMO;
			ai.job_type_minor = JM_FIND_AMMO;
			change_job_status(ai, name, "Finding ammunition.");
			return;
		}

		// Butcher corpses
		if (ai.permitted_work[JOB_BUTCHER] && butcher_and_corpses_exist()) {
			change_settler_glyph(entity, vchar{1, rltk::colors::RED, rltk::colors::BLACK});
			ai.job_type_major = JOB_BUTCHERING;
			ai.job_type_minor = JM_BUTCHER_FIND_CORPSE;
			change_job_status(ai, name, "Finding corpse to butcher.", true);
			return;
		}

		// Hunt
		if (butcher_exist() && ai.permitted_work[JOB_HUNTING] && ranged_status.first && has_ammo && !get_hunting_candidates(pos).empty()) {
			change_settler_glyph(entity, vchar{1, rltk::colors::GREEN, rltk::colors::BLACK});
			ai.job_type_major = JOB_HUNT;
			ai.job_type_minor = JM_HUNT_FIND_TARGET;
			change_job_status(ai, name, "Finding target to hunt.", true);
			return;
		}

		// If we don't have a melee weapon, and one is available, equip it
		if (designations->standing_order_upgrade > standing_orders::SO_UPGRADE_NEVER && is_item_category_available(WEAPON_MELEE) && has_melee_weapon(entity)==false) {
			change_settler_glyph(entity, vchar{1, rltk::colors::WHITE, rltk::colors::BLACK});
			ai.job_type_major = JOB_EQUIP_MELEE;
			ai.job_type_minor = JM_FIND_MELEE_WEAPON;
			change_job_status(ai, name, "Finding a melee weapon.", false);
			return;
		}

		// Look for improved armor
		if (designations->standing_order_upgrade > standing_orders::SO_UPGRADE_NEVER) {
			int max_range = -1;
			if (designations->standing_order_upgrade == standing_orders::SO_UPGRADE_NEARBY) max_range = 15;
			boost::optional<std::size_t> better_armor = find_armor_upgrade(entity, max_range);
			if (better_armor) {
				change_settler_glyph(entity, vchar{1, rltk::colors::WHITE, rltk::colors::BLACK});
				ai.job_type_major = JOB_EQUIP_ARMOR;
				ai.job_type_minor = JM_FIND_ARMOR;
				ai.target_id = better_armor.get();
				change_job_status(ai, name, "Finding armor.", true);
				return;
			}
		}

	} else if (ai.job_type_major == JOB_MINE) {
		do_mining(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_CHOP) {
		do_chopping(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_CONST) {
		do_building(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_REACTION) {
		do_reaction(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_EQUIP_MELEE) {
		do_equip_melee(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_EQUIP_RANGED) {
		do_equip_ranged(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_EQUIP_AMMO) {
		do_equip_ammo(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_EQUIP_ARMOR) {
		do_equip_armor(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_HUNT) {
		do_hunting(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_BUTCHERING) {
		do_butchering(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_GUARD) {
		do_guard_duty(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_DECONSTRUCT) {
		do_deconstruction(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_DEMOLISH) {
		do_demolition(entity, ai, stats, species, pos, name);
		return;
	}
	wander_randomly(entity, pos);
}

void settler_ai_system::do_mining(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {

	if (ai.job_type_minor == JM_FIND_PICK) {
		auto pick = claim_closest_item_by_category(TOOL_DIGGING, pos);
		if (pick==0) {
			cancel_action(e, ai, stats, species, pos, name, "No available pick");
			return;
		}
		auto pick_pos = get_item_location(pick);
		if (!pick_pos) {
			cancel_action(e, ai, stats, species, pos, name, "No available pick");
			return;
		}
		ai.current_path = find_path(pos, *pick_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No available pick");
			return;
		}
		ai.job_type_minor = JM_GO_TO_PICK;
		ai.target_x = pick_pos->x;
		ai.target_y = pick_pos->y;
		ai.target_z = pick_pos->z;
		change_job_status(ai, name, "Traveling to digging tool");
		ai.current_tool = pick;
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_PICK) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_PICK;
				change_job_status(ai, name, "Collect digging tool");
			}, // On arrival
			[&ai, &name, &e, &stats, &species, &pos] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No available pick");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_PICK) {
		// Find the pick, remove any position or stored components, add a carried_by component
		emit(pickup_item_message{ai.current_tool, e.id});

		ai.job_type_minor = JM_GO_TO_SITE;
		change_job_status(ai, name, "Travelling to dig site");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_SITE) {
		const auto idx = mapidx(pos.x, pos.y, pos.z);
		if (mining_map[idx]==0) {
			// We're at a diggable site
			ai.job_type_minor = JM_DIG;
			change_job_status(ai, name, "Digging");
			return;
		}
		// Look at adjacent mining map entries, and path to the closest one. If there is no option,
		// Drop the pick.
		int current_direction = 0;
		uint8_t min_value = std::numeric_limits<uint8_t>::max();
		if (mining_map[mapidx(pos.x, pos.y-1, pos.z)] < min_value && current_region->tile_flags[idx].test(CAN_GO_NORTH)) { 
			min_value = mining_map[mapidx(pos.x, pos.y-1, pos.z)]; 
			current_direction = 1; 
		}
		if (mining_map[mapidx(pos.x, pos.y+1, pos.z)] < min_value && current_region->tile_flags[idx].test(CAN_GO_SOUTH)) { 
			min_value = mining_map[mapidx(pos.x, pos.y+1, pos.z)]; 
			current_direction = 2; 
		}
		if (mining_map[mapidx(pos.x-1, pos.y, pos.z)] < min_value && current_region->tile_flags[idx].test(CAN_GO_WEST)) { 
			min_value = mining_map[mapidx(pos.x-1, pos.y, pos.z)]; 
			current_direction = 3; 
		}
		if (mining_map[mapidx(pos.x+1, pos.y, pos.z)] < min_value && current_region->tile_flags[idx].test(CAN_GO_EAST)) { 
			min_value = mining_map[mapidx(pos.x+1, pos.y, pos.z)]; 
			current_direction = 4; 
		}
		if (mining_map[mapidx(pos.x, pos.y, pos.z-1)] < min_value && current_region->tile_flags[idx].test(CAN_GO_DOWN)) { 
			min_value = mining_map[mapidx(pos.x, pos.y, pos.z-1)]; 
			current_direction = 5; 
		}
		if (mining_map[mapidx(pos.x, pos.y, pos.z+1)] < min_value && current_region->tile_flags[idx].test(CAN_GO_UP)) { 
			min_value = mining_map[mapidx(pos.x, pos.y, pos.z+1)]; 
			current_direction = 6; 
		}

		if (current_direction == 0) {
			ai.job_type_minor = JM_DROP_PICK;
			change_job_status(ai, name, "Dropping digging tools due to being lost!");
			return;
		}

		position_t original = pos;
		switch (current_direction) {
			case 1 : --pos.y; break;
			case 2 : ++pos.y; break;
			case 3 : --pos.x; break;
			case 4 : ++pos.x; break;
			case 5 : --pos.z; break;
			case 6 : ++pos.z; break;
		}
		move_to(e, original, pos);

		return;
	}

	if (ai.job_type_minor == JM_DIG) {
		auto skill_check = skill_roll(e.id, stats, rng, "Mining", DIFICULTY_TOUGH);
		if (skill_check >= SUCCESS) {
			// Determine the digging target from here
			// Make a skill roll, and if successful complete the action
			// When complete, move to dropping the pick
			const auto idx = mapidx(pos.x, pos.y, pos.z);
			const int target_idx = mining_targets[idx];
			const int target_operation = designations->mining[target_idx];
			
			if (target_operation > 0) {
				emit(perform_mining_message{mining_targets[idx], designations->mining[target_idx], pos.x, pos.y, pos.z});
				designations->mining.erase(target_idx);
			}
			if (designations->mining.empty()) {
				ai.job_type_minor = JM_DROP_PICK;
			} else {
				ai.job_type_minor = JM_GO_TO_SITE;
			}
			change_job_status(ai, name, "Dropping digging tools - work complete");
			return;
		} else {
			// Failed!
			if (skill_check == CRITICAL_FAIL) emit_deferred(inflict_damage_message{e.id, 1, "Mining Accident"});
			return;
		}
	}

	if (ai.job_type_minor == JM_DROP_PICK) {
		if (ai.current_tool == 0) std::cout << "Warning: pick is unassigned at this time\n";
		drop_current_tool(e, ai, pos);

		become_idle(e, ai, name);
		return;
	}
}

void settler_ai_system::do_chopping(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {

	if (ai.job_type_minor == JM_FIND_AXE) {
		auto axe = claim_closest_item_by_category(TOOL_CHOPPING, pos);
		if (axe==0) {
			cancel_action(e, ai, stats, species, pos, name, "No available axe");
			return;
		}
		auto axe_pos = get_item_location(axe);
		if (!axe_pos) {
			cancel_action(e, ai, stats, species, pos, name, "No available axe");
			return;
		}
		ai.current_path = find_path(pos, *axe_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to available axe");
			return;
		}
		ai.job_type_minor = JM_GO_TO_AXE;
		ai.target_x = axe_pos->x;
		ai.target_y = axe_pos->y;
		ai.target_z = axe_pos->z;
		change_job_status(ai, name, "Traveling to chopping tool");
		ai.current_tool = axe;
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_AXE) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_AXE;
				change_job_status(ai, name, "Collect chopping tool");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to available axe");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_AXE) {
		// Find the pick, remove any position or stored components, add a carried_by component
		emit(pickup_item_message{ai.current_tool, e.id});

		ai.job_type_minor = JM_FIND_TREE;
		change_job_status(ai, name, "Pathing to chopping site");
		return;
	}

	if (ai.job_type_minor == JM_FIND_TREE) {
		position_t tree_pos = designations->chopping.begin()->second;
		ai.target_id = designations->chopping.begin()->first;

		std::array<position_t, 4> target;
		target[0] = position_t{ tree_pos.x, tree_pos.y-1, tree_pos.z };
		target[1] = position_t{ tree_pos.x, tree_pos.y+1, tree_pos.z };
		target[2] = position_t{ tree_pos.x-1, tree_pos.y, tree_pos.z };
		target[3] = position_t{ tree_pos.x+1, tree_pos.y, tree_pos.z };

		int n = 0;
		while (!ai.current_path && n<4) {
			ai.current_path = find_path(pos, target[n]);
			if (ai.current_path->success) {
				ai.target_x = target[n].x;
				ai.target_y = target[n].y;
				ai.target_z = target[n].z;
			} else {
				ai.current_path.reset();
			}
			++n;
		}

		if (ai.current_path) {
			ai.job_type_minor = JM_GO_TO_TREE;
			change_job_status(ai, name, "Travel to chopping site");
			designations->chopping.erase(ai.target_id);
			return;
		} else {
			designations->chopping.erase(ai.target_id);
			cancel_action(e, ai, stats, species, pos, name, "Can't find tree");
			return;
		}
	}

	if (ai.job_type_minor == JM_GO_TO_TREE) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_CHOP;
				change_job_status(ai, name, "Chopping tree");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to tree");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_CHOP) {
		// Chop down the tree
		auto skill_check = skill_roll(e.id, stats, rng, "Lumberjacking", DIFICULTY_TOUGH);

		if (skill_check >= SUCCESS) {
			// Tree is going down!
			int number_of_logs = 0;
			for (int z=0; z<REGION_DEPTH; ++z) {
				for (int y=0; y<REGION_HEIGHT; ++y) {
					for (int x=0; x<REGION_WIDTH; ++x) {
						const auto idx = mapidx(x,y,z);
						if (current_region->tree_id[idx] == ai.target_id) {
							current_region->solid[idx]=false;
							current_region->tile_flags[idx].reset(CAN_STAND_HERE);
							current_region->tree_id[idx] = 0;
							current_region->tile_type[idx] = tile_type::OPEN_SPACE;
							current_region->tile_calculate(x,y,z);
							++number_of_logs;
						}
					}
				}
			}
			const int tree_idx = mapidx(ai.target_x, ai.target_y, ai.target_z);
			current_region->tile_type[tree_idx] = tile_type::FLOOR;
			current_region->tile_calculate(ai.target_x, ai.target_y, ai.target_z);			

			// Spawn wooden logs
			number_of_logs = (number_of_logs/20)+1;
			for (int i=0; i<number_of_logs; ++i) {
				spawn_item_on_ground(ai.target_x, ai.target_y, ai.target_z, "wood_log", get_material_by_tag("wood"));
			}

			// Update pathing
			for (int Z=-2; Z<10; ++Z) {
				for (int Y=-10; Y<10; ++Y) {
					for (int X=-10; X<10; ++X) {
						current_region->tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
						current_region->tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
					}
				}
			}

			// Change status to drop axe
			emit_deferred(map_dirty_message{});
			emit_deferred(inventory_changed_message{});
			ai.job_type_minor = JM_DROP_AXE;
			change_job_status(ai, name, "Dropping axe");
		} else if (skill_check == CRITICAL_FAIL) {
			// Damage yourself
			emit_deferred(inflict_damage_message{e.id, 1, "Lumberjacking Accident"});
		}
		return;
	}

	if (ai.job_type_minor == JM_DROP_AXE) {
		if (ai.current_tool == 0) std::cout << "Warning: axe is unassigned at this time\n";
		drop_current_tool(e, ai, pos);
		become_idle(e, ai, name);
		return;
	}
}

void settler_ai_system::do_building(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_SELECT_COMPONENT) {
		bool has_components = true;
		for (std::pair<std::size_t, bool> &component : ai.building_target.get().component_ids) {
			if (!component.second) {
				has_components = false;
				ai.current_tool = component.first;
				auto item_loc = get_item_location(ai.current_tool);
				ai.current_path = find_path(pos, *item_loc, true);
				if (ai.current_path->success) {
					component.second = true;
					ai.job_type_minor = JM_GO_TO_COMPONENT;
					change_job_status(ai, name, "Traveling to building component");
				} else {
					cancel_action(e, ai, stats, species, pos, name, "Component unavailable");
					designations->buildings.push_back(*ai.building_target);
					ai.building_target.reset();
				}
				return;
			}
		}

		if (has_components) {
			ai.job_type_minor = JM_ASSEMBLE;
			change_job_status(ai, name, "Constructing building");
		}
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_COMPONENT) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_COMPONENT;
				change_job_status(ai, name, "Collect building component");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to component");
				designations->buildings.push_back(*ai.building_target);
				ai.building_target.reset();
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_COMPONENT) {
		// Find the component, remove any position or stored components, add a carried_by component
		emit(pickup_item_message{ai.current_tool, e.id});

		ai.job_type_minor = JM_GO_TO_BUILDING;
		change_job_status(ai, name, "Going to building site");
		ai.current_path = find_path(pos, position_t{ai.building_target.get().x, ai.building_target.get().y, ai.building_target.get().z}, true);
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_BUILDING) {
		const float distance = distance2d(pos.x, pos.y, ai.building_target.get().x, ai.building_target.get().y );
		const bool same_z = pos.z == ai.building_target.get().z;
		if (pos == ai.current_path->destination || (same_z && distance < 1.4F)) {
			// We're at the site
			ai.current_path.reset();
			ai.job_type_minor = JM_DROP_COMPONENT;
			change_job_status(ai, name, "Dropping building component");
			return;
		}

		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to building");
				designations->buildings.push_back(*ai.building_target);
				ai.building_target.reset();
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_DROP_COMPONENT) {
		if (ai.current_tool == 0) std::cout << "Warning: component is unassigned at this time\n";
		drop_current_tool(e, ai, pos);
		ai.current_tool = 0;
		ai.job_type_minor = JM_SELECT_COMPONENT;
		change_job_status(ai, name, "Reading building plans");
		return;
	}

	if (ai.job_type_minor == JM_ASSEMBLE) {
		std::string tag = ai.building_target.get().tag;
		auto finder = building_defs.find(tag);
		if (finder == building_defs.end()) throw std::runtime_error("Building tag unknown!");

		// Make a skill roll
		const std::string skill = finder->second.skill.first;
		const int difficulty = finder->second.skill.second;
		auto skill_check = skill_roll(e.id, stats, rng, skill, difficulty);

		if (skill_check >= SUCCESS) {
			// Destroy components
			std::size_t material = 0;
			for (auto &comp : ai.building_target.get().component_ids) {
				auto component_ptr = entity(comp.first);
				if (component_ptr) {
					std::string comptag = component_ptr->component<item_t>()->item_tag;
					material = component_ptr->component<item_t>()->material;
					delete_item(comp.first);
					entity(ai.building_target.get().building_entity)->component<building_t>()->built_with.push_back(std::make_pair(comptag, material));
				}
			}

			// Place the building, and assign any provide tags
			entity(ai.building_target.get().building_entity)->component<building_t>()->complete = true;
			emit(opacity_changed_message{});

			for (const building_provides_t &provides : finder->second.provides) {
				if (provides.provides == provides_sleep) {
					entity(ai.building_target.get().building_entity)->assign(construct_provides_sleep_t{});
				} else if (provides.provides == provides_light) {
					entity(ai.building_target.get().building_entity)->assign(lightsource_t{provides.radius, provides.color});
				} else if (provides.provides == provides_wall || provides.provides == provides_floor 
						|| provides.provides == provides_stairs_up
						|| provides.provides == provides_stairs_down || provides.provides == provides_stairs_updown 
						|| provides.provides == provides_ramp) 
				{					
					emit(perform_construction_message{ai.building_target.get().building_entity, tag, material});
				}
			}
			if (finder->second.emits_smoke) {
				entity(ai.building_target.get().building_entity)->assign(smoke_emitter_t{});
			}

			emit_deferred(renderables_changed_message{});
			emit_deferred(inventory_changed_message{});
			emit_deferred(update_workflow_message{});

			// Become idle
			become_idle(e, ai, name);
		}
	}
}

void settler_ai_system::do_reaction(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_SELECT_INPUT) {
		// If there are no inputs, go to the workshop
		auto reactor_pos = entity(ai.reaction_target.get().building_id)->component<position_t>();
		if (ai.reaction_target.get().components.empty() && ! (pos == *reactor_pos)) {
			ai.job_type_minor = JM_GO_TO_WORKSHOP;
			change_job_status(ai, name, ai.reaction_target.get().job_name + std::string(" (Travel)"));
			ai.current_path = find_path(pos, position_t{reactor_pos->x, reactor_pos->y, reactor_pos->z});
			return;
		}

		bool has_components = true;
		for (std::pair<std::size_t, bool> &component : ai.reaction_target.get().components) {
			if (!component.second) {
				has_components = false;
				ai.current_tool = component.first;
				ai.current_path = find_path(pos, *get_item_location(ai.current_tool));
				if (ai.current_path->success) {
					component.second = true;
					ai.job_type_minor = JM_GO_TO_INPUT;
					change_job_status(ai, name, ai.reaction_target.get().job_name + std::string(" (Travel)"));
				} else {
					cancel_action(e, ai, stats, species, pos, name, "Component unavailable");
				}
				return;
			}
		}

		if (has_components) {
			ai.job_type_minor = JM_REACT;
			change_job_status(ai, name, ai.reaction_target.get().job_name);
		}
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_INPUT) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_INPUT;
				change_job_status(ai, name, ai.reaction_target.get().job_name + std::string(" (Collect)"));
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to component");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_INPUT) {
		// Find the component, remove any position or stored components, add a carried_by component
		emit(pickup_item_message{ai.current_tool, e.id});

		ai.job_type_minor = JM_GO_TO_WORKSHOP;
		change_job_status(ai, name, ai.reaction_target.get().job_name + std::string(" (Travel)"));
		auto reactor_pos = entity(ai.reaction_target.get().building_id)->component<position_t>();
		ai.current_path = find_path(pos, position_t{reactor_pos->x, reactor_pos->y, reactor_pos->z});
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_WORKSHOP) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_DROP_INPUT;
				change_job_status(ai, name, ai.reaction_target.get().job_name + std::string(" (Drop)"));
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to workshop");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_DROP_INPUT) {
		if (ai.current_tool == 0) std::cout << "Warning: component is unassigned at this time\n";
		drop_current_tool(e, ai, pos);
		ai.current_tool = 0;
		ai.job_type_minor = JM_SELECT_INPUT;
		change_job_status(ai, name, ai.reaction_target.get().job_name + std::string(" (Planning)"));
		return;
	}

	if (ai.job_type_minor == JM_REACT) {
		// Skill check, destroy inputs, create outputs
		auto finder = reaction_defs.find(ai.reaction_target.get().reaction_tag);
		auto skill_check = skill_roll(e.id, stats, rng, finder->second.skill, finder->second.difficulty);

		if (skill_check >= SUCCESS) {
			// Delete components
			std::size_t material = get_material_by_tag("plasteel");
			for (auto comp : ai.reaction_target.get().components) {
				material = entity(comp.first)->component<item_t>()->material;
				delete_item(comp.first);
			}

			// Spawn results
			for (auto &output : finder->second.outputs) {
				for (int i=0; i<output.second; ++i) {
					spawn_item_on_ground(pos.x, pos.y, pos.z, output.first, material);
				}
			}

			// Emit smoke
			if (finder->second.emits_smoke) emit(emit_particles_message{1, pos.x, pos.y, pos.z});

			// Consume power
			if (finder->second.power_drain != 0) emit(power_consumed_message{finder->second.power_drain});

			// Finish
			free_workshop(ai.reaction_target.get().building_id);
			emit(renderables_changed_message{});
			emit(inventory_changed_message{});

			// Become idle
			become_idle(e, ai, name);
		}
	}
}

void settler_ai_system::do_equip_melee(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_MELEE_WEAPON) {
		int max_range = -1;
		if (designations->standing_order_upgrade == standing_orders::SO_UPGRADE_NEARBY) max_range = 15;
		auto axe = claim_closest_item_by_category(WEAPON_MELEE, pos, max_range);
		if (axe==0) {
			cancel_action(e, ai, stats, species, pos, name, "No available melee weapon");
			return;
		}
		auto axe_pos = get_item_location(axe);
		ai.current_path = find_path(pos, *axe_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to available melee weapon");
			return;
		}
		ai.job_type_minor = JM_GO_TO_MELEE_WEAPON;
		ai.target_x = axe_pos->x;
		ai.target_y = axe_pos->y;
		ai.target_z = axe_pos->z;
		change_job_status(ai, name, "Traveling to melee weapon");
		ai.current_tool = axe;
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_MELEE_WEAPON) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_MELEE_WEAPON;
				change_job_status(ai, name, "Collect melee weapon");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to weapon");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_MELEE_WEAPON) {
		// Find the pick, remove any position or stored components, add a carried_by component
		emit(pickup_item_message{ai.current_tool, e.id, EQUIP_MELEE});
		ai.current_tool = 0;

		become_idle(e, ai, name);
		return;
	}	
}

void settler_ai_system::do_equip_ranged(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_RANGED_WEAPON) {
		int max_range = -1;
		if (designations->standing_order_upgrade == standing_orders::SO_UPGRADE_NEARBY) max_range = 15;
		auto axe = claim_closest_item_by_category(WEAPON_RANGED, pos, max_range);
		if (axe==0) {
			cancel_action(e, ai, stats, species, pos, name, "No available ranged weapon");
			return;
		}
		auto axe_pos = get_item_location(axe);
		ai.current_path = find_path(pos, *axe_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to available ranged weapon");
			return;
		}
		ai.job_type_minor = JM_GO_TO_RANGED_WEAPON;
		ai.target_x = axe_pos->x;
		ai.target_y = axe_pos->y;
		ai.target_z = axe_pos->z;
		change_job_status(ai, name, "Traveling to ranged weapon");
		ai.current_tool = axe;
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_RANGED_WEAPON) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_RANGED_WEAPON;
				change_job_status(ai, name, "Collect ranged weapon");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to weapon");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_RANGED_WEAPON) {
		// Find the pick, remove any position or stored components, add a carried_by component
		emit(pickup_item_message{ai.current_tool, e.id, EQUIP_RANGED});
		ai.current_tool = 0;

		become_idle(e, ai, name);
		return;
	}	
}

void settler_ai_system::do_equip_ammo(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_AMMO) {
		int max_range = -1;
		if (designations->standing_order_upgrade == standing_orders::SO_UPGRADE_NEARBY) max_range = 15;
		auto ranged_status = has_ranged_weapon(e);
		auto axe = claim_closest_ammo(WEAPON_AMMO, pos, ranged_status.second, max_range);
		if (axe==0) {
			cancel_action(e, ai, stats, species, pos, name, "No available ammo");
			return;
		}
		auto axe_pos = get_item_location(axe);
		ai.current_path = find_path(pos, *axe_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to available ammo");
			return;
		}
		ai.job_type_minor = JM_GO_TO_AMMO;
		ai.target_x = axe_pos->x;
		ai.target_y = axe_pos->y;
		ai.target_z = axe_pos->z;
		change_job_status(ai, name, "Traveling to ammo");
		ai.current_tool = axe;
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_AMMO) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_AMMO;
				change_job_status(ai, name, "Collect ammo");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to weapon");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_AMMO) {
		// Find the pick, remove any position or stored components, add a carried_by component
		emit(pickup_item_message{ai.current_tool, e.id, EQUIP_AMMO});
		ai.current_tool = 0;

		become_idle(e, ai, name);
		return;
	}	
}

void settler_ai_system::do_equip_armor(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_ARMOR) {
		auto axe_pos = get_item_location(ai.target_id);
		ai.current_path = find_path(pos, *axe_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to available armor");
			return;
		}
		ai.job_type_minor = JM_GO_TO_ARMOR;
		ai.target_x = axe_pos->x;
		ai.target_y = axe_pos->y;
		ai.target_z = axe_pos->z;
		change_job_status(ai, name, "Traveling to armor");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_ARMOR) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_ARMOR;
				change_job_status(ai, name, "Collect armor");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to armor");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_ARMOR) {
		// Find the pick, remove any position or stored components, add a carried_by component
		auto armor = entity(ai.target_id);
		if (armor) {
			auto item = armor->component<item_t>();
			item_location_t loc = INVENTORY;
			auto finder = clothing_types.find(item->item_tag);
			if (finder->second.slot == "head") loc = HEAD;
			if (finder->second.slot == "torso") loc = TORSO;
			if (finder->second.slot == "legs") loc = LEGS;
			if (finder->second.slot == "shoes") loc = FEET;
			each<item_carried_t>([&e, &pos, &loc](entity_t &E, item_carried_t &c) {
				if (c.carried_by == e.id && c.location == loc) emit(drop_item_message{ E.id, pos.x, pos.y, pos.z });
			});
			emit(pickup_item_message{ static_cast<std::size_t>(ai.target_id), e.id, loc });
			ai.current_tool = 0;
		}

		become_idle(e, ai, name);
		return;
	}	
}

void settler_ai_system::do_hunting(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	auto ranged_status = has_ranged_weapon(e);
	bool has_ammo = has_appropriate_ammo(e, ranged_status.second, pos);
	if (!ranged_status.first || !has_ammo) {
		cancel_action(e, ai, stats, species, pos, name, "Out of ammo");
		return;
	}
	
	if (ai.job_type_minor == JM_HUNT_FIND_TARGET) {
		auto hunting_targets = get_hunting_candidates(pos);
		if (hunting_targets.empty()) {
			cancel_action(e, ai, stats, species, pos, name, "No huntable targets");
			return;
		}

		ai.current_path.reset();
		auto it = hunting_targets.begin();
		while (it != hunting_targets.end() && !ai.current_path) {
			ai.current_path = find_path(pos, it->second);
			if (!ai.current_path) ++it;
		}

		if (ai.current_path) {
			ai.target_x = ai.current_path->destination.x;
			ai.target_y = ai.current_path->destination.y;
			ai.target_z = ai.current_path->destination.z;
			ai.job_type_minor = JM_HUNT;
			ai.job_status = "Hunting";
			return;
		}
	}

	if (ai.job_type_minor == JM_HUNT) {
		if (pos == ai.current_path->destination) {
			// We're at the destination
			ai.current_path.reset();
			ai.job_type_minor = JM_HUNT_FIND_TARGET;
			change_job_status(ai, name, "Finding hunting targets");
			return;
		}
		// Travel to destination
		if (!ai.current_path) {
			cancel_action(e, ai, stats, species, pos, name, "No huntable targets");
		}
		position_t next_step = ai.current_path->steps.front();
		move_to(e, pos, next_step);
		ai.current_path->steps.pop_front();
		return;
	}
}

void settler_ai_system::do_butchering(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
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
		const auto organic_idx = get_material_by_tag("organic");

		auto finder = get_creature_def(corpse->creature_tag);
		for (int i=0; i<finder.yield_bone; ++i) spawn_item_on_ground(pos.x, pos.y, pos.z, "bone", organic_idx);
		for (int i=0; i<finder.yield_hide; ++i) spawn_item_on_ground(pos.x, pos.y, pos.z, "hide", organic_idx);
		for (int i=0; i<finder.yield_meat; ++i) spawn_item_on_ground(pos.x, pos.y, pos.z, "meat", organic_idx);
		for (int i=0; i<finder.yield_skull; ++i) spawn_item_on_ground(pos.x, pos.y, pos.z, "skull", organic_idx);

		delete_entity(ai.targeted_hostile); // Destroy the corpse
		become_idle(e, ai, name);
		return;
	}
}

void settler_ai_system::do_guard_duty(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_GUARDPOST) {
		position_t guard_pos = position_t{ai.target_x, ai.target_y, ai.target_z};
		ai.current_path = find_path(pos, guard_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to guard post");
			return;
		}
		ai.job_type_minor = JM_GO_TO_GUARDPOST;
		change_job_status(ai, name, "Traveling to guard post.");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_GUARDPOST) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_GUARD;
				change_job_status(ai, name, "Guarding");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				cancel_action(e, ai, stats, species, pos, name, "No route to guardpost");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_GUARD) {
		if (shooting_range(e, pos) == 0) cancel_action(e, ai, stats, species, pos, name, "No ammunition");
	}
}

void settler_ai_system::do_deconstruction(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_DECONSTRUCT) {
		std::cout << "find\n";
		auto target_building = entity(ai.target_id);
		if (!target_building) {
			cancel_action(e, ai, stats, species, pos, name, "No route to deconstruction");
			return;
		}
		auto target_pos = target_building->component<position_t>();
		if (!target_pos) {
			cancel_action(e, ai, stats, species, pos, name, "No route to deconstruction");
			return;
		}
		ai.target_x = target_pos->x;
		ai.target_y = target_pos->y;
		ai.target_z = target_pos->z;
		ai.current_path = find_path(pos, target_pos.get());
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to deconstruction");
			return;
		}
		ai.job_type_minor = JM_GO_TO_DECONSTRUCT;
		change_job_status(ai, name, "Traveling to demolition.");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_DECONSTRUCT) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_DECONSTRUCT;
				change_job_status(ai, name, "Performing demolition");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				cancel_action(e, ai, stats, species, pos, name, "No route to deconstruction");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_DECONSTRUCT) {
		// Make a skill roll
		const std::string skill = "Construction";
		const int difficulty = 10;
		auto skill_check = skill_roll(e.id, stats, rng, skill, difficulty);
		if (skill_check >= SUCCESS) {
			auto building_entity = entity(ai.target_id);
			if (building_entity) {
				auto building_comp = building_entity->component<building_t>();
				auto building_pos = building_entity->component<position_t>();
				if (building_comp && building_pos) {
					auto finder = building_defs.find(building_comp->tag);
					if (finder != building_defs.end()) {
						for (const std::pair<std::string, std::size_t> &component : building_comp->built_with) {
							spawn_item_on_ground(building_pos->x, building_pos->y, building_pos->z, component.first, component.second);
						}
						delete_entity(ai.target_id);
						ai.target_id = 0;
					}
				}
			}
			become_idle(e, ai, name);
			emit(opacity_changed_message{});
			return;
		} else {
			// Failed!
			if (skill_check == CRITICAL_FAIL) emit_deferred(inflict_damage_message{e.id, 1, "Demolition Accident"});
			return;
		}
	}	
}

void settler_ai_system::do_demolition(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_DEMOLISH) {
		std::tie(ai.target_x, ai.target_y, ai.target_z) = idxmap(ai.target_id);

		int x = ai.target_x;
		int y = ai.target_y;
		int z = ai.target_z;

		if (current_region->tile_flags[mapidx(x-1,y,z)].test(CAN_STAND_HERE)) {
			x=x-1;
		} else if (current_region->tile_flags[mapidx(x+1,y,z)].test(CAN_STAND_HERE)) {
			x=x+1;
		} else if (current_region->tile_flags[mapidx(x,y-1,z)].test(CAN_STAND_HERE)) {
			y=y-1;
		} else if (current_region->tile_flags[mapidx(x,y+1,z)].test(CAN_STAND_HERE)) {
			y=y+1;
		} else if (current_region->tile_flags[mapidx(x,y,z-1)].test(CAN_STAND_HERE)) {
			z=z-1;
		} else if (current_region->tile_flags[mapidx(x,y,z+1)].test(CAN_STAND_HERE)) {
			z=z+1;
		} else {
			cancel_action(e, ai, stats, species, pos, name, "No route to demolition");
			return;
		}

		ai.current_path = find_path(pos, position_t{x, y, z});
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to demolition");
			return;
		}
		ai.job_type_minor = JM_GO_TO_DEMOLISH;
		change_job_status(ai, name, "Traveling to demolition.");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_DEMOLISH) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_DEMOLISH;
				change_job_status(ai, name, "Performing demolition");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				cancel_action(e, ai, stats, species, pos, name, "No route to deconstruction");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_DEMOLISH) {
		// Make a skill roll
		const std::string skill = "Construction";
		const int difficulty = 15;
		auto skill_check = skill_roll(e.id, stats, rng, skill, difficulty);
		if (skill_check >= SUCCESS) {
			current_region->tile_type[ai.target_id] = tile_type::OPEN_SPACE;
			current_region->solid[ai.target_id] = false;
			current_region->tile_flags[ai.target_id].reset(CONSTRUCTION);
			current_region->tile_flags[ai.target_id].reset(CAN_STAND_HERE);
			spawn_item_on_ground(pos.x, pos.y, pos.z, "block", current_region->tile_material[ai.target_id]);
			current_region->revealed[ai.target_id] = true;
			current_region->revealed[mapidx(ai.target_x, ai.target_y, ai.target_z-1)] = true;
			become_idle(e, ai, name);
			// Update pathing
			for (int i=0; i<2; ++i) {
				for (int Z=-2; Z<4; ++Z) {
					for (int Y=-10; Y<10; ++Y) {
						for (int X=-10; X<10; ++X) {
							current_region->tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
						}
					}
				}
			}
			emit_deferred(tile_removed_message{ai.target_x, ai.target_y, ai.target_z});
			
			return;
		} else {
			// Failed!
			if (skill_check == CRITICAL_FAIL) emit_deferred(inflict_damage_message{e.id, 1, "Demolition Accident"});
			return;
		}
	}
}
