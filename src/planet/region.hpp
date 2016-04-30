#pragma once

#include <vector>
#include <bitset>
#include <rltk.hpp>

constexpr int REGION_WIDTH=512;
constexpr int REGION_HEIGHT=512;
constexpr int REGION_DEPTH=128;
constexpr int TILE_OPTIONS_COUNT = 14;

namespace tile_type {

constexpr uint8_t NOTHING = 0;
constexpr uint8_t SEMI_MOLTEN_ROCK = 1;
constexpr uint8_t ROCK = 2; // Note that this will expand greatly later
constexpr uint8_t SOIL = 3;
constexpr uint8_t YELLOW_SAND = 4;
constexpr uint8_t WHITE_SAND = 5;
constexpr uint8_t RED_SAND = 6;

}

namespace tile_content {

constexpr uint16_t NOTHING = 0;

// Types
constexpr uint16_t PERMAFROST_WHITE = 1;
constexpr uint16_t PERMAFROST_BLUE = 2;
constexpr uint16_t PERMAFROST_CRACKED = 3;
constexpr uint16_t PERMAFROST_DIRTY = 4;
constexpr uint16_t YELLOW_SAND = 5;
constexpr uint16_t YELLOW_SAND_CRACKED = 6;
constexpr uint16_t YELLOW_SAND_DIRTY = 7;
constexpr uint16_t YELLOW_SAND_RIPPLES = 8;
constexpr uint16_t RED_SAND = 9;
constexpr uint16_t RED_SAND_CRACKED = 10;
constexpr uint16_t RED_SAND_DIRTY = 11;
constexpr uint16_t RED_SAND_RIPPLES = 12;
constexpr uint16_t PEBBLES = 13;
constexpr uint16_t WHITE_SAND = 14;
constexpr uint16_t GRAVEL = 15;
constexpr uint16_t MUD = 16;

// Vegetations
constexpr uint16_t LYCHEN = 1024;
constexpr uint16_t MOSS = 1025;
constexpr uint16_t CACTUS = 1026;
constexpr uint16_t GRASS = 1027;
constexpr uint16_t GRASS_SPARSE = 1028;
constexpr uint16_t REEDS = 1029;
constexpr uint16_t GORSE = 1030;
constexpr uint16_t WILDFLOWER = 1031;
constexpr uint16_t THISTLE = 1032;
constexpr uint16_t SHRUB = 1033;
constexpr uint16_t HEATHER = 1034;

// Constructions
constexpr uint16_t CORDEX = 4096;
constexpr uint16_t SCANNER_CONSOLE = 4097;
constexpr uint16_t DEFENSE_CONSOLE = 4098;
constexpr uint16_t EDUCATION_CONSOLE = 4099;
constexpr uint16_t COMMUNICATIONS_CONSOLE = 4100;

}

namespace tile_flags {

constexpr int SOLID = 1;
constexpr int TREE = 2;
constexpr int CONSTRUCTION = 3;

}

struct tile_t {
	uint8_t base_type;
	uint16_t contents;
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

void save_region(const region_t &region);
region_t load_region(const int region_x, const int region_y);

