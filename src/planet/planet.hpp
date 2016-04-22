#pragma once

#include <string>
#include <vector>

constexpr int WORLD_HEIGHT = 512;
constexpr int WORLD_WIDTH = 512;
constexpr double WORLD_PERLIN_DIVISOR = WORLD_HEIGHT/4.0;

enum block_type_t { WATER, PLAINS, HILLS, MOUNTAINS };
enum climate_t {ARCTIC, SUBARCTIC, COLD, TEMPERATE, WARM, HOT, SCORCHING};
enum biome_type_t { UNASSIGNED, GLACIER, ICY_SEA, DUST_SEA, OCEAN, TUNDRA, SWAMP, FLATLAND, WOODS, FOREST,
	JUNGLE, DESERT, BADLANDS, HIGH_TUNDRA, HILL_BOG, HIGH_DESERT, HIGH_BADLANDS, NORMAL_MOUNTAINS };

struct biome_t {
	biome_type_t biome_type;
	std::string name;
	int mean_temperature;
	climate_t climate;
};

struct block_t {
	int height;
	block_type_t type;
	int biome_idx;
};

struct planet_t {
	std::string name;

	int rng_seed;
	int perlin_seed;

	int water_height;
	int plains_height;
	int hills_height;

	std::vector<biome_t> biomes;
	std::vector<block_t> landblocks;
	inline int idx(const int x, const int y) { return y*WORLD_WIDTH + x; }
};

void save_planet();
planet_t load_planet();