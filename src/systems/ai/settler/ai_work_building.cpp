#include "ai_work_building.hpp"
#include "../../../components/ai_tags/ai_tag_work_building.hpp"
#include "../../../main/game_designations.hpp"
#include "../../../utils/telemetry.hpp"
#include "ai_work_template.hpp"
#include "job_board.hpp"
#include "../../../messages/inventory_changed_message.hpp"
#include "../../../messages/map_dirty_message.hpp"
#include "../../../messages/perform_construction.hpp"
#include "../../../messages/update_workflow_message.hpp"
#include "../../../components/game_stats.hpp"
#include "../../../raws/buildings.hpp"
#include "../inventory_system.hpp"
#include "../path_finding.hpp"
#include "../../../components/building.hpp"
#include "../../../components/construct_provides_sleep.hpp"
#include "../../../components/construct_provides_door.hpp"
#include "../../../components/lightsource.hpp"
#include "../../../components/receives_signal.hpp"
#include "../../../components/lever.hpp"
#include "../../../components/smoke_emitter.hpp"

namespace jobs_board {
    void evaluate_building(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
        if (designations->buildings.empty()) return; // Nothing to do

        const auto &designation = designations->buildings.back();
        auto building_id = designation.building_entity;
        const auto building = entity(building_id);
        if (building != nullptr) {
            auto bpos = building->component<position_t>();
            if (bpos != nullptr) {
                const float distance = distance3d(pos.x, pos.y, pos.z, bpos->x, bpos->y, bpos->z);
                board.insert(std::make_pair(distance, jt));
            }
        }
    }
}

void ai_work_building::configure() {
    // Register with the jobs board
    jobs_board::register_job_offer<ai_tag_work_building>(jobs_board::evaluate_building);
}

void ai_work_building::update(const double duration_ms) {
    ai_work_template<ai_tag_work_building> work;
    work.do_ai([this, &work] (entity_t &e, ai_tag_work_building &b, ai_tag_my_turn_t &t, position_t &pos) {
        if (b.step == ai_tag_work_building::building_steps::SELECT_BUILDING) {
            // Select the building
            if (designations->buildings.empty()) {
                work.cancel_work_tag(e);
                return;
            }
            b.building_target = (designations->buildings.back());
            designations->buildings.pop_back();
            b.step = ai_tag_work_building::building_steps::SELECT_COMPONENT;
            return;
        } else if (b.step == ai_tag_work_building::building_steps::SELECT_COMPONENT) {
            // Component selection
            bool has_components = true;
            for (std::pair<std::size_t, bool> &component : b.building_target.component_ids) {
                if (!component.second) {
                    has_components = false;
                    b.current_tool = component.first;
                    auto item_loc = get_item_location(b.current_tool);
                    if (!item_loc) {
                        designations->buildings.push_back(b.building_target);
                        work.cancel_work_tag(e);
                        return;
                    }
                    b.current_path = find_path(pos, *item_loc);
                    if (b.current_path->success) {
                        component.second = true;
                        b.step = ai_tag_work_building::building_steps::GO_TO_COMPONENT;
                        return;
                    } else {
                        designations->buildings.push_back(b.building_target);
                        work.cancel_work_tag(e);
                    }
                    return;
                }
            }

            if (has_components) {
                b.step = ai_tag_work_building::building_steps::ASSEMBLE;
            }
            return;
        } else if (b.step == ai_tag_work_building::building_steps::GO_TO_COMPONENT) {
            work.follow_path(b, pos, e, [&b] () {
                // Cancel
                b.current_path.reset();
                b.step = ai_tag_work_building::building_steps ::SELECT_COMPONENT;
            }, [&b] () {
                // Success
                b.current_path.reset();
                b.step = ai_tag_work_building::building_steps ::COLLECT_COMPONENT;
            });
            return;
        } else if (b.step == ai_tag_work_building::building_steps::COLLECT_COMPONENT) {
            emit(pickup_item_message{b.current_tool, e.id});
            b.step = ai_tag_work_building::building_steps ::GO_TO_BUILDING;
            return;
        } else if (b.step == ai_tag_work_building::building_steps::GO_TO_BUILDING) {
            auto building_entity = entity(b.building_target.building_entity);
            if (!building_entity) {
                work.cancel_work_tag(e);
                return;
            }
            auto bpos = building_entity->component<position_t>();
            if (!bpos) {
                work.cancel_work_tag(e);
                return;
            }

            if (!b.current_path) {
                b.current_path = find_path(pos, *bpos);
                if (!b.current_path->success) {
                    work.cancel_work_tag(e);
                    return;
                }
            }

            // Are we there yet?
            const float distance = distance2d(pos.x, pos.y, bpos->x, bpos->y);
            const bool same_z = pos.z == b.building_target.z;
            if (pos == b.current_path->destination || (same_z && distance < 1.4F)) {
                // We're at the site
                b.current_path.reset();
                b.step = ai_tag_work_building::building_steps::DROP_COMPONENT;
                return;
            }

            work.follow_path(b, pos, e, [&b, &e, &work] () {
                // Cancel
                unclaim_by_id(b.current_tool);
                designations->buildings.push_back(b.building_target);
                work.cancel_work_tag(e);
            }, [&b] () {
                // Success - do nothing
            });

            return;
        } else if (b.step == ai_tag_work_building::building_steps::DROP_COMPONENT) {
            emit(drop_item_message{b.current_tool, pos.x, pos.y, pos.z});
            b.current_tool = 0;
            b.step = ai_tag_work_building::building_steps::SELECT_COMPONENT;
            return;
        } else if (b.step == ai_tag_work_building::building_steps::ASSEMBLE) {
            // Build it!
            std::string tag = b.building_target.tag;
            auto finder = building_defs.find(tag);
            if (finder == building_defs.end()) throw std::runtime_error("Building tag unknown!");

            // Make a skill roll
            const std::string skill = finder->second.skill.first;
            const int difficulty = finder->second.skill.second;
            auto stats = e.component<game_stats_t>();
            auto skill_check = skill_roll(e.id, *stats, rng, skill, difficulty);

            if (skill_check >= SUCCESS) {
                // Destroy components
                std::size_t material = 0;
                for (auto &comp : b.building_target.component_ids) {
                    auto component_ptr = entity(comp.first);
                    if (component_ptr) {
                        std::string comptag = component_ptr->component<item_t>()->item_tag;
                        material = component_ptr->component<item_t>()->material;
                        delete_item(comp.first);
                        entity(b.building_target.building_entity)->component<building_t>()->built_with.push_back(
                                std::make_pair(comptag, material));
                    }
                }

                // Place the building, and assign any provide tags
                call_home("new_building", finder->second.tag);
                entity(b.building_target.building_entity)->component<building_t>()->complete = true;
                emit(opacity_changed_message{});

                for (const building_provides_t &provides : finder->second.provides) {
                    if (provides.provides == provides_sleep) {
                        entity(b.building_target.building_entity)->assign(construct_provides_sleep_t{});
                    } else if (provides.provides == provides_light) {
                        entity(b.building_target.building_entity)->assign(
                                lightsource_t{provides.radius, provides.color});
                    } else if (provides.provides == provides_door) {
                        entity(b.building_target.building_entity)->assign(construct_door_t{});
                    } else if (provides.provides == provides_wall || provides.provides == provides_floor
                               || provides.provides == provides_stairs_up
                               || provides.provides == provides_stairs_down ||
                               provides.provides == provides_stairs_updown
                               || provides.provides == provides_ramp || provides.provides == provides_stonefall_trap
                               || provides.provides == provides_cage_trap || provides.provides == provides_blades_trap
                               || provides.provides == provides_spikes) {
                        emit(perform_construction_message{b.building_target.building_entity, tag, material});
                    } else if (provides.provides == provides_signal_recipient) {
                        entity(b.building_target.building_entity)->assign(receives_signal_t{});
                    } else if (provides.provides == provides_lever) {
                        entity(b.building_target.building_entity)->assign(lever_t{});
                    }
                }
                if (finder->second.emits_smoke) {
                    entity(b.building_target.building_entity)->assign(smoke_emitter_t{});
                }

                emit_deferred(renderables_changed_message{});
                emit_deferred(inventory_changed_message{});
                emit_deferred(update_workflow_message{});
                emit(map_changed_message{});

                // Become idle
                work.cancel_work_tag(e);
            }

            return;
        }
    });
}