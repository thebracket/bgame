#pragma once

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
	bool render_flat = false;
};
