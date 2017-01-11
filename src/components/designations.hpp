#pragma once

#include <rltk.hpp>
#include <boost/container/flat_map.hpp>
#include "../planet/region.hpp"
#include "position.hpp"
#include "helpers/building_designation_t.hpp"
#include "helpers/unbuild_t.hpp"
#include "helpers/standing_orders.hpp"
#include "helpers/reaction_task_t.hpp"


using namespace rltk;

struct designations_t {

	boost::container::flat_map<int, uint8_t> mining;
	boost::container::flat_map<int, uint8_t> architecture;
	boost::container::flat_map<int, position_t> chopping;
	std::vector<building_designation_t> buildings;
	std::vector<std::pair<uint8_t, std::string>> build_orders;
	std::vector<std::pair<bool, position_t>> guard_points;
	std::vector<unbuild_t> deconstructions;
	std::vector<std::pair<bool, position_t>> harvest;
	int current_power = 10;
	uint64_t current_cash = 100;

	uint8_t standing_order_idle_move = 1;
	uint8_t standing_order_wildlife_treatment = 1;
	uint8_t standing_order_upgrade = 1;

	// Not serialized
	rltk::color_t alert_color = rltk::colors::WHITE;
	int total_capacity = 10;

	designations_t() {
	}

	std::string xml_identity = "designations_t";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};
