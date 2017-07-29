#pragma once

#include <rltk.hpp>
#include "../../components/position.hpp"
#include <string>

bool has_melee_weapon(const entity_t &entity);
std::pair<bool, std::string> has_ranged_weapon(const entity_t &entity);
bool has_appropriate_ammo(const entity_t &entity, const std::string ammo_type, const position_t &pos);
int shooting_range(entity_t &entity, const position_t &pos);
int calculate_armor_class(entity_t &entity);
std::size_t get_melee_id(const entity_t &entity);
std::pair<std::size_t, std::size_t> get_ranged_and_ammo_id(const entity_t &entity);
int get_weapon_initiative_penalty(const std::size_t &weapon_id);
