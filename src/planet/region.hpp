#pragma once

#include <vector>
#include <bitset>
#include <rltk.hpp>

constexpr int REGION_WIDTH=512;
constexpr int REGION_HEIGHT=512;
constexpr int REGION_DEPTH=128;
constexpr int TILE_OPTIONS_COUNT = 16;

namespace tile_flags {

constexpr int SOLID = 0;
constexpr int TREE = 1;
constexpr int CONSTRUCTION = 2;

constexpr int CAN_GO_UP = 3;
constexpr int CAN_GO_DOWN = 4;
constexpr int CAN_GO_NORTH = 5;
constexpr int CAN_GO_EAST = 6;
constexpr int CAN_GO_SOUTH = 7;
constexpr int CAN_GO_WEST = 8;
constexpr int CAN_STAND_HERE = 9;

constexpr int REVEALED = 10;
constexpr int VISIBLE = 11;

}

struct tile_t {
	uint8_t base_type;
	uint16_t contents;
	std::bitset<TILE_OPTIONS_COUNT> flags;
	rltk::vchar render_as;
	uint16_t tree_id = 0;
};

struct region_t {
	region_t() { tiles.resize(REGION_WIDTH * REGION_HEIGHT * REGION_DEPTH); }
	int region_x, region_y, biome_idx;
	std::vector<tile_t> tiles;
	inline int idx(const int x, const int y, const int z) { return (z * REGION_HEIGHT * REGION_WIDTH) + (y * REGION_WIDTH) + x; }

	inline void set(const int x, const int y, const int z, const uint8_t base, const uint16_t content, const uint8_t liquid=0, 
			const int16_t temperature=0, const bool solid=false) {
		const int loc = idx(x,y,z);
		tiles[loc].base_type = base;
		tiles[loc].contents = content;
		if (solid) {
			tiles[loc].flags.set(tile_flags::SOLID);
		} else {
			tiles[loc].flags.reset(tile_flags::SOLID);
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

