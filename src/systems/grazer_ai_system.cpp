#include "grazer_ai_system.hpp"
#include "../messages/messages.hpp"
#include "../components/grazer_ai.hpp"
#include "../components/viewshed.hpp"
#include "../components/settler_ai.hpp"
#include "../components/sentient_ai.hpp"
#include "../components/renderable.hpp"
#include "../components/health.hpp"

using namespace rltk;

void grazer_ai_system::wander_randomly(entity_t &entity, position_t &original) {
    emit_deferred(entity_wants_to_move_randomly_message{entity.id});
}

void grazer_ai_system::configure() {
    system_name = "Grazer AI System";
    subscribe_mbox<tick_message>();
}

void grazer_ai_system::update(const double ms) {
    std::queue<tick_message> * tick = mbox<tick_message>();
	while (!tick->empty()) {
        tick->pop();

        parallel_each<grazer_ai, position_t, viewshed_t>([this] (entity_t &e, grazer_ai &ai, position_t &pos, viewshed_t &view) {
            if (ai.initiative < 1) {
                // Can we see anything scary?
                bool terrified = false;
                float terror_distance = 1000.0F;
                std::size_t closest_fear = 0;
                for (const std::size_t other_entity : view.visible_entities) {
                    if (!entity(other_entity)) break;
                    if (entity(other_entity)->component<settler_ai_t>() || entity(other_entity)->component<sentient_ai>()) {
                        terrified = true;
                        auto other_pos = entity(other_entity)->component<position_t>();
                        const float d = distance3d(pos.x, pos.y, pos.z, other_pos->x, other_pos->y, other_pos->z);
                        if (d < terror_distance) {
                            terror_distance = d;
                            closest_fear = other_entity;
                        }
                    }
                }
                if (!terrified) {
                    // Handle the AI here
                    const auto idx = mapidx(pos.x, pos.y, pos.z);
                    if (current_region->tile_vegetation_type[idx] > 0) {
                        if (rng.roll_dice(1,6)==1) emit_deferred(vegetation_damage_message{idx, 1});                         
                    } else {
                        this->wander_randomly(e, pos);
                    }
                } else {
                    // Poor creature is scared!
                    if (terror_distance < 1.5F) {
                        // Attack the target
                        auto health = entity(e.id)->component<health_t>();
                        if (health) {
                            if (!health->unconscious) emit_deferred(creature_attack_message{e.id, closest_fear});
                        }
                    } else {
                        emit_deferred(entity_wants_to_flee_message{e.id, closest_fear});
                    }
                }

                ai.initiative = std::max(1, rng.roll_dice(1, 12) - ai.initiative_modifier);
            } else {
                --ai.initiative;
            }
        });
    }
}