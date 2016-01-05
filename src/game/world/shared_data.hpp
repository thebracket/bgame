#pragma once

#include <unordered_map>
#include "location.hpp"

struct available_item_t
{
	string name;
	location_t location;
	int entity_id;
};

struct shared_data_t
{
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
};
