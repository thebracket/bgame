#pragma once

#include "../../components/position.hpp"
#include "../../components/claimed_t.hpp"
#include "../../components/items/item.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../global_assets/game_building.hpp"
#include "../../bengine/ecs.hpp"
#include "../ai/inventory_system.hpp"
#include "../../bengine/geometry.hpp"

namespace inventory {
	bool is_ammo_available(const std::string &ammo_type);
	std::size_t claim_closest_ammo(const int &category, position_t &pos, const std::string &ammo_type, const int range = -1);
	position_t * get_item_location(std::size_t id);
	std::size_t find_armor_upgrade(bengine::entity_t &entity, const int range = -1);

	std::vector<buildings::available_building_t> get_available_buildings();
	std::vector<std::pair<std::string, std::string>> get_available_reactions();

	int available_items_by_tag(const std::string &tag);
	int available_items_by_reaction_input(const int worker, const reaction_input_t &input) noexcept;
	int blocks_available();
	std::size_t claim_item_by_tag(const std::string &tag);
	std::size_t claim_item_by_reaction_input(const reaction_input_t &input, const int worker_id, const bool really_claim = true) noexcept;
	void unclaim_by_id(const std::size_t &id);

	void delete_item(const std::size_t &id);

	template <class C>
	inline int item_category_available() {
		int result = 0;
		bengine::each_without<claimed_t, item_t, C>([&result](bengine::entity_t &e, item_t &i, C &cat) {
			++result;
		});
		return result;
	}

	template<class C>
	inline bool is_item_category_available() {
		return (item_category_available<C>()>0);
	}

	template <class ITEM_TYPE>
	std::size_t find_closest_unclaimed_item_by_category_and_claim_it_immediately(int &claimer_id, position_t &pos) {
		using namespace bengine;

		// We're taking advantage of map being sorted to find the closest here
		std::map<float, std::size_t> distance_sorted;

		// Search items
		each_without<claimed_t, item_t, ITEM_TYPE>([&distance_sorted, &pos](entity_t &e, item_t &i, ITEM_TYPE &type) {
			auto p = get_item_location(e.id);
			if (p) {
				const float distance = distance3d_squared(pos.x, pos.y, pos.z, p->x, p->y, p->z);
				distance_sorted[distance] = e.id;
			}
		});

		if (distance_sorted.empty()) return 0;
		std::size_t closest_matching_id = distance_sorted.begin()->second;
		entity(closest_matching_id)->assign(claimed_t{ claimer_id });
		return closest_matching_id;
	}

	template <class ITEM_TYPE>
	std::size_t claim_closest_item_by_category(position_t &pos, const int range) {
		// We're taking advantage of map being sorted to find the closest here
		std::map<float, std::size_t> distance_sorted;

		bengine::each<item_t, ITEM_TYPE>([&distance_sorted, &pos, &range](bengine::entity_t &e, item_t &i, ITEM_TYPE &type) {
			if (e.component<claimed_t>() == nullptr) {
				auto p = get_item_location(e.id);
				if (p) {
					const float distance = bengine::distance3d_squared(pos.x, pos.y, pos.z, p->x, p->y, p->z);
					if (range == -1 || distance < range) distance_sorted[distance] = e.id;
				}
			}
		});

		if (distance_sorted.empty()) return 0;

		std::size_t closest_matching_id = distance_sorted.begin()->second;
		systems::inventory_system::claim_item(closest_matching_id, true);

		return closest_matching_id;
	}
}
