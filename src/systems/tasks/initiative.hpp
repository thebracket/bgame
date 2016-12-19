#pragma once

#include <rltk.hpp>
#include "../../components/settler_ai.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/initiative.hpp"

namespace tasks {

inline void calculate_initiative(initiative_t &ai, game_stats_t &stats) {
	ai.initiative = std::max(1, rng.roll_dice(1, 12) - stat_modifier(stats.dexterity) + ai.initiative_modifier);
}

}
