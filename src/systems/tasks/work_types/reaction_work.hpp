#pragma once

#include <rltk.hpp>
#include "../../components/settler_ai.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/species.hpp"
#include "../../components/position.hpp"
#include "../../components/name.hpp"

void do_reaction(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
