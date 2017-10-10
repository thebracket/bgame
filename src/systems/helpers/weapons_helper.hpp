#pragma once

#include "../../bengine/ecs.hpp"
#include <utility>
#include "../../components/position.hpp"

namespace weapons {
	bool has_melee_weapon(const bengine::entity_t &entity);
	std::pair<bool, std::string> has_ranged_weapon(const bengine::entity_t &entity);
	bool has_appropriate_ammo(const bengine::entity_t &entity, const std::string ammo_type, const position_t &pos);
	int shooting_range(bengine::entity_t &entity, const position_t &pos);
	int calculate_armor_class(bengine::entity_t &entity);
	std::size_t get_melee_id(const bengine::entity_t &entity);
	std::pair<std::size_t, std::size_t> get_ranged_and_ammo_id(const bengine::entity_t &entity);
	int get_weapon_initiative_penalty(const std::size_t &weapon_id);
}