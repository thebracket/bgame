#include "building_work.hpp"
#include "../../../messages/messages.hpp"
#include "../../ai/path_finding.hpp"
#include "../../ai/inventory_system.hpp"
#include "../idle_mode.hpp"
#include "../settler_glyph.hpp"
#include "../settler_job_status.hpp"
#include "../settler_drop_tool.hpp"
#include "../settler_cancel_action.hpp"
#include "../pathfinding.hpp"
#include "../initiative.hpp"
#include "../../../components/construct_provides_sleep.hpp"
#include "../../../components/smoke_emitter.hpp"
#include "../../../components/lightsource.hpp"
#include "../../../components/construct_provides_door.hpp"
#include "../../../raws/buildings.hpp"
#include "../../../components/receives_signal.hpp"
#include "../../../components/lever.hpp"
#include "../../../utils/telemetry.hpp"
#include "../../main/game_designations.hpp"

using namespace rltk;
using tasks::become_idle;
using tasks::change_settler_glyph;
using tasks::change_job_status;
using tasks::drop_current_tool;
using tasks::cancel_action;
using tasks::follow_path;
using tasks::follow_result_t;
using tasks::calculate_initiative;

void
do_building(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
    if (ai.job_type_minor == JM_SELECT_COMPONENT) {
        bool has_components = true;
        for (std::pair<std::size_t, bool> &component : ai.building_target.component_ids) {
            if (!component.second) {
                has_components = false;
                ai.current_tool = component.first;
                auto item_loc = get_item_location(ai.current_tool);
                if (!item_loc) {
                    cancel_action(e, ai, stats, species, pos, name, "Component unavailable");
                    designations->buildings.push_back(ai.building_target);
                    ai.has_building_target = false;
                    return;
                }
                ai.current_path = find_path(pos, *item_loc, true);
                if (ai.current_path->success) {
                    component.second = true;
                    ai.job_type_minor = JM_GO_TO_COMPONENT;
                    change_job_status(ai, name, "Traveling to building component");
                } else {
                    cancel_action(e, ai, stats, species, pos, name, "Component unavailable");
                    designations->buildings.push_back(ai.building_target);
                    ai.has_building_target = false;
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
                        []() {}, // Do nothing on success
                        [&ai, &name]() {
                            ai.current_path.reset();
                            ai.job_type_minor = JM_COLLECT_COMPONENT;
                            change_job_status(ai, name, "Collect building component");
                        }, // On arrival
                        [&e, &ai, &stats, &species, &pos, &name]() {
                            unclaim_by_id(ai.current_tool);
                            cancel_action(e, ai, stats, species, pos, name, "No route to component");
                            designations->buildings.push_back(ai.building_target);
                            ai.has_building_target = false;
                        }
        );
        return;
    }

    if (ai.job_type_minor == JM_COLLECT_COMPONENT) {
        // Find the component, remove any position or stored components, add a carried_by component
        emit(pickup_item_message{ai.current_tool, e.id});

        ai.job_type_minor = JM_GO_TO_BUILDING;
        change_job_status(ai, name, "Going to building site");
        ai.current_path = find_path(pos, position_t{ai.building_target.x, ai.building_target.y, ai.building_target.z},
                                    true);
        return;
    }

    if (ai.job_type_minor == JM_GO_TO_BUILDING) {
        if (!ai.current_path) {
            ai.current_path = find_path(pos,
                                        position_t{ai.building_target.x, ai.building_target.y, ai.building_target.z},
                                        true);
        }
        if (!ai.current_path->success) {
            unclaim_by_id(ai.current_tool);
            cancel_action(e, ai, stats, species, pos, name, "No route to building");
            designations->buildings.push_back(ai.building_target);
            ai.has_building_target = false;
        }
        const float distance = distance2d(pos.x, pos.y, ai.building_target.x, ai.building_target.y);
        const bool same_z = pos.z == ai.building_target.z;
        if (pos == ai.current_path->destination || (same_z && distance < 1.4F)) {
            // We're at the site
            ai.current_path.reset();
            ai.job_type_minor = JM_DROP_COMPONENT;
            change_job_status(ai, name, "Dropping building component");
            return;
        }

        tasks::try_path(e, ai, pos,
                        []() {}, // Do nothing on success
                        [&ai, &name]() {}, // On arrival
                        [&e, &ai, &stats, &species, &pos, &name]() {
                            unclaim_by_id(ai.current_tool);
                            cancel_action(e, ai, stats, species, pos, name, "No route to building");
                            designations->buildings.push_back(ai.building_target);
                            ai.has_building_target = false;
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
        std::string tag = ai.building_target.tag;
        auto finder = building_defs.find(tag);
        if (finder == building_defs.end()) throw std::runtime_error("Building tag unknown!");

        // Make a skill roll
        const std::string skill = finder->second.skill.first;
        const int difficulty = finder->second.skill.second;
        auto skill_check = skill_roll(e.id, stats, rng, skill, difficulty);

        if (skill_check >= SUCCESS) {
            // Destroy components
            std::size_t material = 0;
            for (auto &comp : ai.building_target.component_ids) {
                auto component_ptr = entity(comp.first);
                if (component_ptr) {
                    std::string comptag = component_ptr->component<item_t>()->item_tag;
                    material = component_ptr->component<item_t>()->material;
                    delete_item(comp.first);
                    entity(ai.building_target.building_entity)->component<building_t>()->built_with.push_back(
                            std::make_pair(comptag, material));
                }
            }

            // Place the building, and assign any provide tags
            call_home("new_building", finder->second.tag);
            entity(ai.building_target.building_entity)->component<building_t>()->complete = true;
            emit(opacity_changed_message{});

            for (const building_provides_t &provides : finder->second.provides) {
                if (provides.provides == provides_sleep) {
                    entity(ai.building_target.building_entity)->assign(construct_provides_sleep_t{});
                } else if (provides.provides == provides_light) {
                    entity(ai.building_target.building_entity)->assign(lightsource_t{provides.radius, provides.color});
                } else if (provides.provides == provides_door) {
                    entity(ai.building_target.building_entity)->assign(construct_door_t{});
                } else if (provides.provides == provides_wall || provides.provides == provides_floor
                           || provides.provides == provides_stairs_up
                           || provides.provides == provides_stairs_down || provides.provides == provides_stairs_updown
                           || provides.provides == provides_ramp || provides.provides == provides_stonefall_trap
                           || provides.provides == provides_cage_trap || provides.provides == provides_blades_trap
                           || provides.provides == provides_spikes) {
                    emit(perform_construction_message{ai.building_target.building_entity, tag, material});
                } else if (provides.provides == provides_signal_recipient) {
                    entity(ai.building_target.building_entity)->assign(receives_signal_t{});
                } else if (provides.provides == provides_lever) {
                    entity(ai.building_target.building_entity)->assign(lever_t{});
                }
            }
            if (finder->second.emits_smoke) {
                entity(ai.building_target.building_entity)->assign(smoke_emitter_t{});
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
