#pragma once

#include <vector>
#include <bitset>
#include <unordered_map>
#include <unordered_set>

namespace systems {
	namespace stockpile_system {
		struct stockpile_info_t {
			stockpile_info_t() = default;
			stockpile_info_t(const int &new_id, const std::bitset<128> &cat) : id(new_id), category(cat) {}

			int id;
			std::bitset<128> category;
			int free_capacity = 0;
			std::unordered_set<int> open_tiles;
		};

		struct storable_item_t {
			storable_item_t() = default;
			storable_item_t(const int &id, const int &dest) : item_id(id), dest_tile(dest) {}

			int item_id;
			int dest_tile;
			bool deleteme = false;
		};

		extern std::unordered_map<int, stockpile_info_t> stockpiles;
		extern std::unordered_map<int, std::vector<int>> stockpile_targets;
		extern std::vector<storable_item_t> storable_items;

		void run(const double &duration_ms);
	}
}