#pragma once

#include <rltk.hpp>
#include "../../components/settler_ai.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/species.hpp"
#include "../../components/position.hpp"
#include "../../components/name.hpp"

inline rltk::color_t get_task_color(const std::string &skill) {
	if (skill == "Masonry") {
		return rltk::colors::LightGrey;
	} else if (skill == "Carpentry") {
		return rltk::colors::BurlyWood;
	} else if (skill == "Furnace Operation") {
		return rltk::colors::Orange;
	} else if (skill == "Glassmaking") {
		return rltk::colors::CYAN;
	} else if (skill == "Metalworking") {
		return rltk::colors::BRASS;
	}
	return rltk::colors::Grey;
}

void do_work_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
