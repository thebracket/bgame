#pragma once

#include <vector>
#include <bitset>

constexpr int REGION_WIDTH=512;
constexpr int REGION_HEIGHT=512;
constexpr int REGION_DEPTH=128;
constexpr int TILE_OPTIONS_COUNT = 14;

struct tile_t {
	uint8_t base_type;
	uint8_t contents;
	uint8_t liquid;
	int16_t temperature;
	std::bitset<TILE_OPTIONS_COUNT> flags;
};

struct region_t {
	int region_x, region_y, biome_idx;
	std::vector<tile_t> tiles(REGION_WIDTH * REGION_HEIGHT * REGION_DEPTH);
	inline int idx(const int x, const int y) { return y*REGION_WIDTH + x; }
};
