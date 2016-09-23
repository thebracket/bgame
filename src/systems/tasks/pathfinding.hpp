#pragma once

#include <rltk.hpp>
#include <functional>
#include "../../components/position.hpp"
#include "../../main/game_globals.hpp"
#include "../../messages/entity_moved_message.hpp"
#include "../../messages/renderables_changed_message.hpp"

namespace tasks {

enum follow_result_t { SUCCESS, AT_DESTINATION, FAIL };

inline bool can_enter_tile(const position_t &pos) {
    const int idx = mapidx(pos);
    return current_region->tile_flags[idx].test(CAN_STAND_HERE);
}

template<class T>
inline follow_result_t follow_path(const rltk::entity_t &e, T &ai, position_t &pos) {
    if (!ai.current_path) return FAIL;
    if (pos == ai.current_path->destination) return AT_DESTINATION;
    if (ai.current_path->steps.empty()) return AT_DESTINATION;

    const position_t next_step = ai.current_path->steps.front();
    ai.current_path->steps.pop_front();
    if (can_enter_tile(next_step)) {
        emit_deferred(entity_wants_to_move_message{e.id, next_step});
		emit_deferred(renderables_changed_message{});
        return SUCCESS;
    } else {
        return FAIL;
    }
}

template<class T>
inline void try_path(const rltk::entity_t &e, const T &ai, position_t &pos, 
    const std::function<void()> &on_success, 
    const std::function<void()> &on_reached, 
    const std::function<void()> &on_fail)
{
    switch (follow_path(e, ai, pos)) {
        case SUCCESS : { on_success(); } break;
        case AT_DESTINATION : { on_reached(); } break;
        case FAIL : { on_fail(); } break;
    }
}

}
