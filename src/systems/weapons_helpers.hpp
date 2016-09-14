#pragma once

#include <rltk.hpp>
#include "../components/position.hpp"
#include <string>

bool has_melee_weapon(const entity_t &entity);
std::pair<bool, std::string> has_ranged_weapon(const entity_t &entity);
bool has_appropriate_ammo(const entity_t &entity, const std::string ammo_type, const position_t &pos);
int shooting_range(const entity_t &entity, const position_t &pos);
