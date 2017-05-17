#include "mining_system.hpp"
#include "../../messages/messages.hpp"
#include "../../planet/region.hpp"
#include "../../main/game_designations.hpp"

std::vector<uint8_t> mining_map;
std::vector<int> mining_targets;

using namespace region;

void walk_mining_map(const int x, const int y, const int z, const int distance, const int IDX) {
	if (distance > 250) return;
	const auto idx = mapidx(x,y,z);
	if (mining_map[idx] > distance) {
		if (!flag(idx, CAN_STAND_HERE)) return;
		mining_map[idx] = distance;
		mining_targets[idx] = IDX;

		if (flag(idx, CAN_GO_NORTH)) { walk_mining_map(x,y-1,z,distance+1,IDX); }
		if (flag(idx, CAN_GO_SOUTH)) { walk_mining_map(x,y+1,z,distance+1,IDX); }
		if (flag(idx, CAN_GO_EAST)) { walk_mining_map(x+1,y,z,distance+1,IDX); }
		if (flag(idx, CAN_GO_WEST)) { walk_mining_map(x-1,y,z,distance+1,IDX); }
		if (flag(idx, CAN_GO_UP)) { walk_mining_map(x,y,z+1,distance+1,IDX); }
		if (flag(idx, CAN_GO_DOWN)) { walk_mining_map(x,y,z-1,distance+1,IDX); }
	}
}

void make_mining_map() {
	// Start by setting all cells to -1
	std::fill(mining_map.begin(), mining_map.end(), std::numeric_limits<uint8_t>::max());
	std::fill(mining_targets.begin(), mining_targets.end(), std::numeric_limits<int>::max());

	// Now we iterate the designations, and build a list of starting points
	std::vector<std::tuple<int,int,int,int>> starting_points;
	for (int z=1; z<REGION_DEPTH-2; ++z) {
		for (int y=1; y<REGION_HEIGHT-2; ++y) {
			for (int x=1; x<REGION_WIDTH-2; ++x) {
				const auto idx = mapidx(x,y,z);
				auto mf = designations->mining.find(idx);
				if (mf != designations->mining.end()) {
					if (mf->second==1) {					
						// Regular digging - can stand on a neighboring square horizontally
						starting_points.push_back(std::make_tuple(x-1,y,z,idx));
						starting_points.push_back(std::make_tuple(x+1,y,z,idx));
						starting_points.push_back(std::make_tuple(x,y-1,z,idx));
						starting_points.push_back(std::make_tuple(x,y+1,z,idx));
					} else if (mf->second==2) {					
						// Channeling - can stand on the square or the square below or a spot neighboring the top
						starting_points.push_back(std::make_tuple(x,y,z,idx));
						starting_points.push_back(std::make_tuple(x,y,z-1,idx));
						starting_points.push_back(std::make_tuple(x-1,y,z,idx));
						starting_points.push_back(std::make_tuple(x+1,y,z,idx));
						starting_points.push_back(std::make_tuple(x,y-1,z,idx));
						starting_points.push_back(std::make_tuple(x,y+1,z,idx));
					} else if (mf->second==3) {					
						// Ramping - can stand on the square or the square above or a spot neighboring the bottom
						starting_points.push_back(std::make_tuple(x,y,z,idx));
						starting_points.push_back(std::make_tuple(x,y,z+1,idx));
						starting_points.push_back(std::make_tuple(x-1,y,z-1,idx));
						starting_points.push_back(std::make_tuple(x+1,y,z-1,idx));
						starting_points.push_back(std::make_tuple(x,y-1,z-1,idx));
						starting_points.push_back(std::make_tuple(x,y+1,z-1,idx));
					} else if (mf->second==4) {	
						// Up stairs - can stand on the square or the square above
						starting_points.push_back(std::make_tuple(x,y,z,idx));
						starting_points.push_back(std::make_tuple(x,y,z+1,idx));
						starting_points.push_back(std::make_tuple(x-1,y,z,idx));
						starting_points.push_back(std::make_tuple(x+1,y,z,idx));
						starting_points.push_back(std::make_tuple(x,y-1,z,idx));
						starting_points.push_back(std::make_tuple(x,y+1,z,idx));
					} else if (mf->second==5) {	
						// Down stairs - can stand on the square or the square below, or adjacent to the top
						starting_points.push_back(std::make_tuple(x,y,z,idx));
						starting_points.push_back(std::make_tuple(x,y,z-1,idx));
						starting_points.push_back(std::make_tuple(x-1,y,z,idx));
						starting_points.push_back(std::make_tuple(x+1,y,z,idx));
						starting_points.push_back(std::make_tuple(x,y-1,z,idx));
						starting_points.push_back(std::make_tuple(x,y+1,z,idx));
					} else if (mf->second==6) {	
						// Up/down Stairs - can stand on the square or above/below
						starting_points.push_back(std::make_tuple(x,y,z,idx));
						starting_points.push_back(std::make_tuple(x,y,z-1,idx));
						starting_points.push_back(std::make_tuple(x,y,z+1,idx));
						starting_points.push_back(std::make_tuple(x-1,y,z,idx));
						starting_points.push_back(std::make_tuple(x+1,y,z,idx));
						starting_points.push_back(std::make_tuple(x,y-1,z,idx));
						starting_points.push_back(std::make_tuple(x,y+1,z,idx));
					}
				}
			}
		}
	}

	// Now we iterate through each point, and update the distance map
	for (auto &loc : starting_points) {
		walk_mining_map(std::get<0>(loc), std::get<1>(loc), std::get<2>(loc), 0, std::get<3>(loc));
	}
}

void mining_system::update(const double duration_ms) {
	if (dirty) {
		make_mining_map();
	}
	dirty = false;
}

void mining_system::configure() {
	system_name = "Mining";
	mining_map.resize(REGION_WIDTH * REGION_HEIGHT * REGION_DEPTH);
	mining_targets.resize(REGION_WIDTH * REGION_HEIGHT * REGION_DEPTH);
	subscribe<recalculate_mining_message>([this](recalculate_mining_message &msg) {
		make_mining_map();
	});
}
