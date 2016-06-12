#include "path_finding.hpp"
#include <rltk.hpp>
#include "../planet/region.hpp"
#include "../game_globals.hpp"

struct navigator_t {
	static float get_distance_estimate(position_t &pos, position_t &goal) {
		float d = distance3d_squared(pos.x, pos.y, pos.z, goal.x, goal.y, goal.z);
		return d;
	}

	static bool is_goal(position_t &pos, position_t &goal) {
		return pos == goal;
	}

	// This is where we calculate where you can go from a given tile. In this case, we check
	// all 8 directions, and if the destination is walkable return it as an option.
	static bool get_successors(position_t pos, std::vector<position_t> &successors) {
		const int idx = mapidx(pos.x, pos.y, pos.z);
		if (current_region->tiles[idx].flags.test(tile_flags::CAN_GO_NORTH)) successors.push_back(position_t(pos.x, pos.y-1, pos.z));
		if (current_region->tiles[idx].flags.test(tile_flags::CAN_GO_SOUTH)) successors.push_back(position_t(pos.x, pos.y+1, pos.z));
		if (current_region->tiles[idx].flags.test(tile_flags::CAN_GO_EAST)) successors.push_back(position_t(pos.x+1, pos.y, pos.z));
		if (current_region->tiles[idx].flags.test(tile_flags::CAN_GO_WEST)) successors.push_back(position_t(pos.x-1, pos.y, pos.z));
		if (current_region->tiles[idx].flags.test(tile_flags::CAN_GO_UP)) successors.push_back(position_t(pos.x, pos.y, pos.z+1));
		if (current_region->tiles[idx].flags.test(tile_flags::CAN_GO_DOWN)) successors.push_back(position_t(pos.x, pos.y, pos.z-1));

		return true;
	}

	// This function lets you set a cost on a tile transition. For now, we'll always use a cost of 1.0.
	static float get_cost(position_t &position, position_t &successor) {
		return 1.0f;
	}

	// This is a simple comparison to determine if two locations are the same. It just passes
	// through to the location_t's equality operator in this instance (we didn't do that automatically)
	// because there are times you might want to behave differently.
	static bool is_same_state(position_t &lhs, position_t &rhs) {
		return lhs == rhs;
	}
};

std::shared_ptr<rltk::navigation_path<position_t>> find_path(const position_t &start, const position_t &end) {
	return rltk::find_path<position_t, navigator_t>(start, end);
}