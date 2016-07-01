#pragma once

#include <vector>
#include <bitset>
#include <rltk.hpp>

constexpr int REGION_WIDTH=512;
constexpr int REGION_HEIGHT=512;
constexpr int REGION_DEPTH=128;
constexpr int REGION_TILES_COUNT = REGION_WIDTH * REGION_HEIGHT * REGION_DEPTH;

namespace tile_flags {

constexpr uint16_t TREE = 2;
constexpr uint16_t CONSTRUCTION = 4;

constexpr uint16_t CAN_GO_UP = 8;
constexpr uint16_t CAN_GO_DOWN = 16;
constexpr uint16_t CAN_GO_NORTH = 32;
constexpr uint16_t CAN_GO_EAST = 64;
constexpr uint16_t CAN_GO_SOUTH = 128;
constexpr uint16_t CAN_GO_WEST = 256;
constexpr uint16_t CAN_STAND_HERE = 512;

}

struct bitset16 {
	uint16_t bits;
	constexpr void set(const uint16_t n) { bits |= n; }
	constexpr void reset(const uint16_t n) { bits &= ~n; }
	constexpr bool test(const uint16_t n) { return bits & n; }
};

struct tile_t {
	uint8_t base_type;
	uint16_t contents;
	bitset16 flags;
	rltk::vchar render_as;
	uint16_t tree_id = 0;
};

constexpr std::size_t mapidx(const int x, const int y, const int z) { 
	return (z * REGION_HEIGHT * REGION_WIDTH) + (y * REGION_WIDTH) + x; 
}

struct region_t {
	region_t() { 
		tiles.resize(REGION_TILES_COUNT);
		revealed.resize(REGION_TILES_COUNT);
		visible.resize(REGION_TILES_COUNT); 
		solid.resize(REGION_TILES_COUNT);
	}

	int region_x, region_y, biome_idx;
	std::vector<tile_t> tiles;
	std::vector<bool> revealed;
	std::vector<bool> visible;
	std::vector<bool> solid;

	inline void set(const int x, const int y, const int z, const uint8_t base, const uint16_t content, const uint8_t liquid=0, 
		const int16_t temperature=0, const bool is_solid=false) {
		const int loc = mapidx(x,y,z);
		tiles[loc].base_type = base;
		tiles[loc].contents = content;
		if (is_solid) {
			solid[loc] = true;
		} else {
			solid[loc] = false;
		}
	}

	void determine_tile_standability(const int &x, const int &y, const int &z);
	void determine_tile_connectivity(const int &x, const int &y, const int &z);
	void determine_connectivity();

	void calculate_render_tiles();
	void calculate_render_tile(const int &idx);

	int next_tree_id = 1;
};

void save_region(const region_t &region);
region_t load_region(const int region_x, const int region_y);

