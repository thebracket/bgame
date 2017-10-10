#pragma once

#include "../../components/position.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../global_assets/game_building.hpp"
#include "../../bengine/ecs.hpp"
#include <string>

namespace inventory {
	int item_category_available(const int &category);
	bool is_item_category_available(const int &category);
	bool is_ammo_available(const std::string &ammo_type);
	std::size_t claim_closest_item_by_category(const int &category, position_t &pos, const int range = -1);
	std::size_t claim_closest_ammo(const int &category, position_t &pos, const std::string &ammo_type, const int range = -1);
	position_t * get_item_location(std::size_t id);
	std::size_t find_armor_upgrade(bengine::entity_t &entity, const int range = -1);

	std::vector<buildings::available_building_t> get_available_buildings();
	std::vector<std::pair<std::string, std::string>> get_available_reactions();

	int available_items_by_tag(const std::string &tag);
	int available_items_by_reaction_input(const reaction_input_t &input);
	int blocks_available();
	std::size_t claim_item_by_tag(const std::string &tag);
	std::size_t claim_item_by_reaction_input(const reaction_input_t &input, bool really_claim = true);
	void unclaim_by_id(const std::size_t &id);

	void delete_item(const std::size_t &id);
}