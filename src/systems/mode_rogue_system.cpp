#include "mode_rogue_system.hpp"

#include "../messages/messages.hpp"
#include "path_finding.hpp"
#include "movement_system.hpp"
#include "weapons_helpers.hpp"
#include <rltk.hpp>

void mode_rogue_system::settler_calculate_initiative(settler_ai_t &ai, game_stats_t &stats) {
	ai.initiative = std::max(1, rng.roll_dice(1, 12) - stat_modifier(stats.dexterity));
}

void mode_rogue_system::configure() {
    system_name = "Rogue Mode System";
    subscribe<tick_message>([this](tick_message &msg) {
        if (game_master_mode != ROGUE) return;

        entity_t * settler = entity(selected_settler);
        if (settler == nullptr) {
            // The settler has died!
            game_master_mode = PLAY;
            pause_mode = PAUSED;
            return;
        }
        settler_ai_t * ai = settler->component<settler_ai_t>();
        health_t * health = settler->component<health_t>();
        game_stats_t * stats = settler->component<game_stats_t>();

        if (ai->initiative < 1) {
            if (ai->job_type_major == JOB_IDLE) {
                // We're waiting for input
                pause_mode = PAUSED;
            } else {
                // Perform the in-progress job
                if (health->unconscious) {
                    pause_mode = RUNNING;
                    return;
                } else if (ai->job_type_major == JOB_ROGUE_GOTO) {
                    if (!ai->current_path || ai->current_path->success == false || ai->current_path->steps.size() == 0) {
                        ai->job_type_major = JOB_IDLE;
                        ai->job_status = "Idle";
                        pause_mode = PAUSED;
                        return;
                    }
                    position_t next_step = ai->current_path->steps.front();
                    emit(entity_wants_to_move_message{settler->id, next_step});
                    ai->current_path->steps.pop_front();

                } else if (ai->job_type_major == JOB_ROGUE_BASH) {
                    emit(settler_attack_message{settler->id, ai->targeted_hostile});
                    ai->job_type_major = JOB_IDLE;
                    ai->job_status = "Idle";
                    pause_mode = PAUSED;
                    return;
                } else if (ai->job_type_major == JOB_ROGUE_SHOOT) {
                    emit(settler_ranged_attack_message{settler->id, ai->targeted_hostile});
                    ai->job_type_major = JOB_IDLE;
                    ai->job_status = "Idle";
                    pause_mode = PAUSED;
                }

                settler_calculate_initiative(*ai, *stats);
            }
        } else {
            --ai->initiative;
        }
    });
}

void mode_rogue_system::update(const double ms) {
    if (game_master_mode != ROGUE) return;

    int mouse_x, mouse_y;
    int font_w, font_h;
    std::tie(mouse_x, mouse_y) = get_mouse_position();
    std::tie(font_w, font_h) = term(1)->get_font_size();
    const int terminal_x = mouse_x / font_w;
	const int terminal_y = mouse_y / font_h;
	const int world_x = std::min(clip_left + terminal_x, REGION_WIDTH);
	const int world_y = std::min(clip_top + terminal_y-2, REGION_HEIGHT);
	const int tile_idx = mapidx( world_x, world_y, camera_position->region_z );

    entity_t * settler = entity(selected_settler);
    settler_ai_t * ai = settler->component<settler_ai_t>();
    position_t * pos = settler->component<position_t>();

    if (ai->job_type_major != JOB_IDLE) {
        // We're doing something, so move the camera
        camera_position->region_x = pos->x;
        camera_position->region_y = pos->y;
        camera_position->region_z = pos->z;
        emit(map_dirty_message{});
    }

    if (ai->job_type_major == JOB_IDLE) {
        ai->targeted_hostile = 0;
        auto entities = entity_octree.find_by_loc(octree_location_t{world_x, world_y, camera_position->region_z});
        for (const auto &id : entities) {
            if (entity(id)->component<grazer_ai>() != nullptr) ai->targeted_hostile = id;
        }
        if (ai->targeted_hostile != 0 && get_mouse_button_state(rltk::button::LEFT)) {
            position_t * target_pos = entity(ai->targeted_hostile)->component<position_t>();
            const float range = distance3d(pos->x, pos->y, pos->z, target_pos->x, target_pos->y, target_pos->z);
            if (range < 1.5F) {
                // Melee attack
                ai->job_type_major = JOB_ROGUE_BASH;
                ai->job_status = "Melee attack";
                emit(renderables_changed_message{});
                pause_mode = RUNNING;
            } else {
                if (shooting_range(*settler, *pos) > range) {
                    ai->job_type_major = JOB_ROGUE_SHOOT;
                    ai->job_status = "Ranged attack";
                    emit(renderables_changed_message{});
                    pause_mode = RUNNING;
                } else {
                    std::cout << "Cannot fire - weapon/range/ammo fail.\n";
                }
            }
        }

        if (ai->targeted_hostile==0 && current_region->tile_flags[tile_idx].test(CAN_STAND_HERE)) {
            ai->current_path.reset();
            ai->current_path = find_path(*pos, position_t{ world_x, world_y, camera_position->region_z });
            if (get_mouse_button_state(rltk::button::LEFT)) {
                ai->job_type_major = JOB_ROGUE_GOTO;
                ai->job_status = "Travel";
                emit(renderables_changed_message{});
                pause_mode = RUNNING;
            }
        }
    }
}