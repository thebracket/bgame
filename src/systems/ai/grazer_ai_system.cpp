#include "grazer_ai_system.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/viewshed.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/health.hpp"
#include "../tasks/threat_scanner.hpp"

using namespace rltk;

void grazer_ai_system::wander_randomly(entity_t &entity, position_t &original) {
    emit_deferred(entity_wants_to_move_randomly_message{entity.id});
}

void grazer_ai_system::on_message(const tick_message &msg) {
    parallel_each<grazer_ai, position_t, viewshed_t>([this] (entity_t &e, grazer_ai &ai, position_t &pos, viewshed_t &view) {
        if (ai.initiative < 1) {
            // Can we see anything scary?
            auto hostile = tasks::can_see_hostile(e, pos, view, [] (entity_t &other) {
                if (other.component<settler_ai_t>() || other.component<sentient_ai>()) {
                    return true;
                } else {
                    return false;
                }
            });

            if (!hostile.terrified) {
                // Handle the AI here
                const auto idx = mapidx(pos.x, pos.y, pos.z);
                if (current_region->tile_vegetation_type[idx] > 0) {
                    if (rng.roll_dice(1,6)==1) emit_deferred(vegetation_damage_message{idx, 1});
                } else {
                    this->wander_randomly(e, pos);
                }
            } else {
                // Poor creature is scared!
                if (hostile.terror_distance < 1.5F) {
                    // Attack the target
                    auto health = entity(e.id)->component<health_t>();
                    if (health) {
                        if (!health->unconscious) emit_deferred(creature_attack_message{e.id, hostile.closest_fear});
                    }
                } else {
                    emit_deferred(entity_wants_to_flee_message{e.id, hostile.closest_fear});
                }
            }

            ai.initiative = std::max(1, rng.roll_dice(1, 12) - ai.initiative_modifier);
        } else {
            --ai.initiative;
        }
    });
}