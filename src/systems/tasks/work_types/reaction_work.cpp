#include "reaction_work.hpp"
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
#include "../world_queries.hpp"
#include "../settler_sleep.hpp"
#include "../settler_wander.hpp"
#include "../settler_move_to.hpp"
#include "../../../raws/reactions.hpp"

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

void do_reaction(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
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
			std::size_t material = get_material_by_tag("plasteel").get();
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
