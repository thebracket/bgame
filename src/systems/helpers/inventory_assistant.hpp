#pragma once

#include "../../components/position.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../global_assets/game_building.hpp"
#include "../../bengine/ecs.hpp"
#include "../../stdafx.h"

namespace inventory {
	template <class C>
	inline int item_category_available() {
		int result = 0;
		each<item_t, C>([&result](bengine::entity_t &e, item_t &i, C &cat) {
			if (e.component<claimed_t>() == nullptr) ++result;
		});
		return result;
	}

	template<class C>
	inline bool is_item_category_available() {
		return (item_category_available<C>()>0);
	}

	template <class ITEM_TYPE>
	std::size_t claim_closest_item_by_category(position_t &pos, const int range) {
		// We're taking advantage of map being sorted to find the closest here
		std::map<float, std::size_t> distance_sorted;

		each<item_t, ITEM_TYPE>([&distance_sorted, &category, &pos, &range](bengine::entity_t &e, item_t &i, ITEM_TYPE &type) {
			if (e.component<claimed_t>() == nullptr) {
				auto p = get_item_location(e.id);
				if (p) {
					const float distance = distance3d_squared(pos.x, pos.y, pos.z, p->x, p->y, p->z);
					if (range == -1 || distance < range) distance_sorted[distance] = e.id;
				}
			}
		});

		if (distance_sorted.empty()) return 0;

		std::size_t closest_matching_id = distance_sorted.begin()->second;
		systems::inventory_system::claim_item(closest_matching_id, true);

		return closest_matching_id;
	}

	bool is_ammo_available(const std::string &ammo_type);
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