#include "mining_system.hpp"
#include "../messages/messages.hpp"
#include "../planet/region.hpp"
#include "../game_globals.hpp"
#include <tuple>

std::vector<int> mining_map;

void walk_mining_map(const int x, const int y, const int z, const int distance) {
	if (distance > 500) return;
	const int idx = current_region.idx(x,y,z);
	if (mining_map[idx] > distance) {
		if (!current_region.tiles[idx].flags.test(tile_flags::CAN_STAND_HERE)) return;
		mining_map[idx] = distance;

		if (current_region.tiles[idx].flags.test(tile_flags::CAN_GO_NORTH)) { walk_mining_map(x,y-1,z,distance+1); }
		if (current_region.tiles[idx].flags.test(tile_flags::CAN_GO_SOUTH)) { walk_mining_map(x,y+1,z,distance+1); }
		if (current_region.tiles[idx].flags.test(tile_flags::CAN_GO_EAST)) { walk_mining_map(x+1,y,z,distance+1); }
		if (current_region.tiles[idx].flags.test(tile_flags::CAN_GO_WEST)) { walk_mining_map(x-1,y,z,distance+1); }
		if (current_region.tiles[idx].flags.test(tile_flags::CAN_GO_UP)) { walk_mining_map(x,y,z+1,distance+1); }
		if (current_region.tiles[idx].flags.test(tile_flags::CAN_GO_DOWN)) { walk_mining_map(x,y,z-1,distance+1); }
	}
}

void make_mining_map() {
	// Start by setting all cells to -1
	std::fill(mining_map.begin(), mining_map.end(), std::numeric_limits<int>::max());

	// Now we iterate the designations, and build a list of starting points
	std::vector<std::tuple<int,int,int>> starting_points;
	for (int z=1; z<REGION_DEPTH-2; ++z) {
		for (int y=1; y<REGION_HEIGHT-2; ++y) {
			for (int x=1; x<REGION_WIDTH-2; ++x) {
				const int idx = current_region.idx(x,y,z);
				if (designations->mining[idx]>0) {
					starting_points.push_back(std::make_tuple(x-1,y,z));
					starting_points.push_back(std::make_tuple(x+1,y,z));
					starting_points.push_back(std::make_tuple(x,y-1,z));
					starting_points.push_back(std::make_tuple(x,y+1,z));
				}
			}
		}
	}

	// Now we iterate through each point, and update the distance map
	for (auto &loc : starting_points) {
		walk_mining_map(std::get<0>(loc), std::get<1>(loc), std::get<2>(loc), 0);
	}
}

void mining_system::update(const double duration_ms) {
	if (dirty) {
		make_mining_map();
	}
	dirty = false;
}

void mining_system::configure() {
	mining_map.resize(REGION_WIDTH * REGION_HEIGHT * REGION_DEPTH);
	subscribe<recalculate_mining_message>([this](recalculate_mining_message &msg) {
		dirty = true;
	});
}
