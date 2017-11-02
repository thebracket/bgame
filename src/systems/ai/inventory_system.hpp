#pragma once

#include <vector>
#include "../../components/item.hpp"
#include "../../components/item_carried.hpp"
#include "../../global_assets/game_building.hpp"

namespace systems {
	namespace inventory_system {
		void run(const double &duration_ms);
		void drop_item(const std::size_t &ID, const int &X, const int &Y, const int &Z);
		void claim_item(const std::size_t i, const bool c);
		void pickup_item(const std::size_t &ID, const std::size_t &holder);
		void pickup_item(const std::size_t &ID, const std::size_t &holder, const item_location_t &LOC);
		void destroy_item(const std::size_t ID);
		void building_request(int x, int y, int z, buildings::available_building_t building);
		void inventory_has_changed();
	}
}