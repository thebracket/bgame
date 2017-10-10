#pragma once

#include <vector>
#include <bitset>
#include <unordered_map>
#include <unordered_set>

namespace systems {
	namespace stockpile_system {
		struct stockpile_info_t {
			stockpile_info_t() {}
			stockpile_info_t(const std::size_t &ID, const std::bitset<128> &cat) : id(ID), category(cat) {}

			std::size_t id;
			std::bitset<128> category;
			int free_capacity = 0;
			std::unordered_set<int> open_tiles;
		};

		struct storable_item_t {
			storable_item_t() {}
			storable_item_t(const std::size_t &id, const int &dest) : item_id(id), dest_tile(dest) {}

			std::size_t item_id;
			int dest_tile;
		};

		extern std::unordered_map<std::size_t, stockpile_info_t> stockpiles;
		extern std::unordered_map<int, std::vector<std::size_t>> stockpile_targets;
		extern std::vector<storable_item_t> storable_items;

		void run(const double &duration_ms);
	}
}