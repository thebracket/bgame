#pragma once

#include <map>

enum mining_type_t { MINE_DIG, MINE_CHANNEL, MINE_RAMP, MINE_STAIRS_UP, MINE_STAIRS_DOWN, MINE_STAIRS_UPDOWN, MINE_DELETE };

struct mining_designations_t {

	std::map<int, uint8_t> mining_targets;
	int brush_type = 0;
	int brush_size_x = 1;
	int brush_size_y = 1;
	int mine_mode = 0;
	int radius = 1;
	bool stairs_helper = true;
	int stairs_depth = 1;
};

