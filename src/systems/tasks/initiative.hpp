#pragma once

#include <rltk.hpp>
#include "../../components/settler_ai.hpp"
#include "../../components/game_stats.hpp"

namespace tasks {

template<class C>
inline void calculate_initiative(C &ai, game_stats_t &stats, const int &initiative_penalty) {
	ai.initiative = std::max(1, rng.roll_dice(1, 12) - stat_modifier(stats.dexterity) + initiative_penalty);
}

}
