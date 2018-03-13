#pragma once

#include <vector>
#include "../../components/items/item.hpp"
#include "../../components/items/item_carried.hpp"
#include "../../global_assets/game_building.hpp"

namespace systems {
	namespace inventory_system {
		void run(const double &duration_ms);
		void drop_item(const int &ID, const int &X, const int &Y, const int &Z);
		void claim_item(const int i, const bool c);
		void pickup_item(const int &ID, const std::size_t &holder);
		void pickup_item(const int &ID, const std::size_t &holder, const item_location_t &LOC);
		void destroy_item(const int ID);
		void building_request(const int x, const int y, const int z, const buildings::available_building_t building);
		void inventory_has_changed();
	}
}