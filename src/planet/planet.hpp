#pragma once

#include <string>
#include <vector>
#include "constants.hpp"
#include "civilizations.hpp"
#include "history.hpp"

namespace block_type {
constexpr uint8_t MAX_BLOCK_TYPE = 9;
constexpr uint8_t NONE = 0;
constexpr uint8_t WATER = 1;
constexpr uint8_t PLAINS = 2;
constexpr uint8_t HILLS = 3;
constexpr uint8_t MOUNTAINS = 4;
constexpr uint8_t MARSH = 5;
constexpr uint8_t PLATEAU = 6;
constexpr uint8_t HIGHLANDS = 7;
constexpr uint8_t COASTAL = 8;
constexpr uint8_t SALT_MARSH = 9;
}

struct block_t {
	uint8_t height = 0;
	uint8_t variance = 0;
	uint8_t type = 0;
	int8_t temperature_c = 0;
	int8_t rainfall = 0;
	int biome_idx = -1;
};

struct biome_t {
	std::size_t type = 0;
	std::string name = "";
	int8_t mean_temperature = 0;
	int8_t mean_rainfall = 0;
	uint8_t mean_altitude = 0;
	uint8_t mean_variance = 0;
	uint8_t warp_mutation = 0;
	uint8_t evil = 0;
	uint8_t savagery = 0;
	int center_x = 0;
	int center_y = 0;
};

struct planet_t {
	std::string name = "Test World";

	int rng_seed;
	int perlin_seed;

	uint8_t water_height;
	uint8_t plains_height;
	uint8_t hills_height;

	std::vector<block_t> landblocks;
	std::vector<biome_t> biomes;
	inline int idx(const int x, const int y) { return y*WORLD_WIDTH + x; }
	civ_holder_t civs;
	history_t history;
};

void save_planet(const planet_t &planet);
planet_t load_planet();