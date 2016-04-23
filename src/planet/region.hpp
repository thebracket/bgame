#pragma once

#include <vector>
#include <bitset>

constexpr int REGION_WIDTH=512;
constexpr int REGION_HEIGHT=512;
constexpr int REGION_DEPTH=128;
constexpr int TILE_OPTIONS_COUNT = 14;

namespace tile_type {

constexpr uint8_t NOTHING = 0;
constexpr uint8_t SEMI_MOLTEN_ROCK = 1;
constexpr uint8_t ROCK = 2; // Note that this will expand greatly later

}

namespace tile_content {

constexpr uint8_t NOTHING = 0;

}

namespace tile_flags {

constexpr int SOLID = 1;

}

struct tile_t {
	uint8_t base_type;
	uint8_t contents;
	uint8_t liquid;
	int16_t temperature;
	std::bitset<TILE_OPTIONS_COUNT> flags;
};

struct region_t {
	region_t() { tiles.resize(REGION_WIDTH * REGION_HEIGHT * REGION_DEPTH); }
	int region_x, region_y, biome_idx;
	std::vector<tile_t> tiles;
	inline int idx(const int x, const int y, const int z) { return (z * REGION_HEIGHT * REGION_WIDTH) + (y * REGION_WIDTH) + x; }

	inline void set(const int x, const int y, const int z, const uint8_t base, const uint8_t content, const uint8_t liquid=0, 
			const int16_t temperature=0, const bool solid=false) {
		const int loc = idx(x,y,z);
		tiles[loc].base_type = base;
		tiles[loc].contents = content;
		tiles[loc].liquid = liquid;
		tiles[loc].temperature = temperature;
		if (solid) {
			tiles[loc].flags.set(tile_flags::SOLID);
		} else {
			tiles[loc].flags.reset(tile_flags::SOLID);
		}
	}
};
