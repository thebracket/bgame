#include "building_work.hpp"
#include "../settler_work_time.hpp"
#include "../../../messages/messages.hpp"
#include "../../../main/game_globals.hpp"
#include "../../path_finding.hpp"
#include "../../mining_system.hpp"
#include "../../inventory_system.hpp"
#include "../../ai/workflow_system.hpp"
#include "../../ai/wildlife_population_system.hpp"
#include "../../damage/weapons_helpers.hpp"
#include "../../components/item_carried.hpp"
#include "../idle_mode.hpp"
#include "../settler_glyph.hpp"
#include "../settler_job_status.hpp"
#include "../settler_drop_tool.hpp"
#include "../settler_cancel_action.hpp"
#include "../pathfinding.hpp"
#include "../initiative.hpp"
#include "../../../messages/log_message.hpp"
#include "../../../components/logger.hpp"
#include "../../../components/health.hpp"
#include "../../../components/renderable.hpp"
#include "../../../components/corpse_harvestable.hpp"
#include "../../../components/construct_provides_sleep.hpp"
#include "../../../components/viewshed.hpp"
#include "../../../components/smoke_emitter.hpp"
#include "../../../components/grazer_ai.hpp"
#include "../../../components/sentient_ai.hpp"
#include "../../../components/lightsource.hpp"
#include "../../../components/falling.hpp"
#include "../../../components/construct_provides_door.hpp"
#include "../world_queries.hpp"
#include "../settler_sleep.hpp"
#include "../settler_wander.hpp"
#include "../settler_move_to.hpp"
#include "../../../raws/buildings.hpp"

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

void do_building(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
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
		if (!ai.current_path) {
            ai.current_path = find_path(pos, position_t{ai.building_target.get().x, ai.building_target.get().y, ai.building_target.get().z}, true);
        }
        if (!ai.current_path->success) {
            unclaim_by_id(ai.current_tool);
            cancel_action(e, ai, stats, species, pos, name, "No route to building");
            designations->buildings.push_back(*ai.building_target);
            ai.building_target.reset();
        }
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
				} else if (provides.provides == provides_door) {
					entity(ai.building_target.get().building_entity)->assign(construct_door_t{});
				} else if (provides.provides == provides_wall || provides.provides == provides_floor 
						|| provides.provides == provides_stairs_up
						|| provides.provides == provides_stairs_down || provides.provides == provides_stairs_updown 
						|| provides.provides == provides_ramp || provides.provides == provides_stonefall_trap
						|| provides.provides == provides_cage_trap || provides.provides == provides_blades_trap)
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
			emit(map_changed_message{});

			// Become idle
			become_idle(e, ai, name);
		}
	}
}
