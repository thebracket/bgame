#pragma once

#include <unordered_map>
#include "../planet/region/region.hpp"
#include "position.hpp"
#include "helpers/building_designation_t.hpp"
#include "helpers/unbuild_t.hpp"
#include "helpers/standing_orders.hpp"
#include "helpers/reaction_task_t.hpp"
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"
#include <boost/container/flat_map.hpp>

struct designations_t {

	boost::container::flat_map<int, uint8_t> mining;
	std::unordered_map<int, uint8_t> architecture;
	std::unordered_map<int, position_t> chopping;
	std::vector<building_designation_t> buildings;
	std::vector<std::pair<uint8_t, std::string>> build_orders;
	std::vector<std::pair<bool, position_t>> guard_points;
	std::vector<unbuild_t> deconstructions;
	std::vector<std::pair<bool, position_t>> harvest;
	std::vector<std::size_t> levers_to_pull;
	int current_power = 10;
	uint64_t current_cash = 100;

	uint8_t standing_order_idle_move = 1;
	uint8_t standing_order_wildlife_treatment = 1;
	uint8_t standing_order_upgrade = 1;

	// Not serialized
	bengine::color_t alert_color{1.0f, 1.0f, 1.0f};
	int total_capacity = 10;

	designations_t() {
	}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( mining, architecture, chopping, buildings, build_orders, guard_points, deconstructions,
			harvest, levers_to_pull, current_power, current_cash, standing_order_idle_move, standing_order_wildlife_treatment,
            standing_order_upgrade); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<designations_t>>)