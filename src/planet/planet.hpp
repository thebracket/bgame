#pragma once
#include <cereal/archives/binary.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/array.hpp>
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

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(height, variance, type, temperature_c, rainfall, biome_idx);
	}
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

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(type, name, mean_temperature, mean_rainfall, mean_altitude, mean_variance, warp_mutation,
			evil, savagery, center_x, center_y);
	}
};

struct river_step_t {
	int x = 0;
	int y = 0;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(x, y);
	}
};

struct river_t {
	std::string name = "A river that will one day have a name";
	int start_x = 0;
	int start_y = 0;
	std::vector<river_step_t> steps;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(name, start_x, start_y, steps);
	}
};

struct planet_t {
	std::string name = "Test World";

	int rng_seed;
	int perlin_seed;
	int remaining_settlers;
	int migrant_counter;
	int water_divisor = 3;
	int plains_divisor = 3;
	int starting_settlers = 10;
	bool strict_beamdown = false;

	uint8_t water_height;
	uint8_t plains_height;
	uint8_t hills_height;

	std::vector<block_t> landblocks;
	std::vector<biome_t> biomes;
	std::vector<river_t> rivers;
	inline int idx(const int x, const int y) { return y * WORLD_WIDTH + x; }
	civ_holder_t civs;
	history_t history;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(name, rng_seed, perlin_seed, remaining_settlers, migrant_counter, water_height, plains_height, hills_height,
			landblocks, biomes, rivers, civs, history, water_divisor, plains_divisor, starting_settlers, strict_beamdown);
	}
};

void save_planet(const planet_t &planet);
planet_t load_planet();