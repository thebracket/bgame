#include "grazer_ai_system.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/viewshed.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/health.hpp"
#include "../tasks/threat_scanner.hpp"
#include "../../messages/entity_moved_message.hpp"

using namespace rltk;

void grazer_ai_system::wander_randomly(entity_t &entity, position_t &original) {
    emit_deferred(entity_wants_to_move_randomly_message{entity.id});
}

void grazer_ai_system::on_message(const action_available_message &msg) {
    auto e = entity(msg.entity_id);
    auto ai = e->component<grazer_ai>();
    if (!ai) return;
    auto pos = e->component<position_t>();
    auto view = e->component<viewshed_t>();
    if (!pos || !view) return;

    // Handle the AI here
    const auto idx = mapidx(pos->x, pos->y, pos->z);
    if (current_region->tile_vegetation_type[idx] > 0) {
        if (rng.roll_dice(1,6)==1) emit_deferred(vegetation_damage_message{idx, 1});
    } else {
        this->wander_randomly(*e, *pos);
        emit_deferred(huntable_moved_message{});
    }
}