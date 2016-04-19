#pragma once

#include <string>
#include <vector>

constexpr int WORLD_HEIGHT = 1024;
constexpr int WORLD_WIDTH = 1024;

struct biome_t {
	int biome_type;
	std::string name;
};

struct block_t {
	int height;
	int biome_idx;
	int mean_temperature_celsius;
};

struct planet_t {
	std::string name;
	long generated;
	int perlin_noise_seed;

	std::vector<biome_t> biomes;
	std::vector<block_t> landblocks;
	inline int idx(const int x, const int y) { return y*WORLD_WIDTH + x; }
};
