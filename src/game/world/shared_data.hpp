#pragma once

#include "world_defs.hpp"
#include <unordered_map>
#include <boost/optional.hpp>
#include <vector>
#include "location.hpp"
#include "../../engine/virtual_terminal.hpp"

struct available_item_t
{
	string name;
	location_t location;
	int entity_id;
};

struct shared_data_t
{
	shared_data_t() {
		render_list_3d.resize( REGION_TILES );
	}

	int camera_handle;
	int cordex_handle;
	int stored_power;
	int starting_system_x;
	int starting_system_y;

	// Non Persistent
	int max_power = 100;
	bool paused = true;
	bool render_graphics = true;
	bool omniscience = false;
	std::unordered_map<string, vector<available_item_t>> inventory;
	std::vector<boost::optional<engine::vterm::screen_character>> render_list_3d;
};
