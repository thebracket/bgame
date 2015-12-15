#pragma once

#include "world_height_map.hpp"
#include "../../../engine/rng.h"
#include "world_gen_hydrology.hpp"
#include "../tile_types.hpp"
#include <memory>
#include <vector>
#include <utility>
#include <iostream>

struct biome_t {
    std::vector<std::pair<uint16_t,uint16_t>> points;
    uint8_t climate;
    uint8_t difficulty;
    uint8_t biome_type;
    
    void dump() {
	std::cout << "Climate: ";
	switch (climate) {
	  case tile_climate::ARCTIC : std::cout << "Arctic"; break;
	  case tile_climate::SUBARCTIC : std::cout << "Sub-arctic"; break;
	  case tile_climate::COLD : std::cout << "Cold"; break;
	  case tile_climate::COOL : std::cout << "Cool"; break;
	  case tile_climate::TEMPERATE : std::cout << "Temperate"; break;
	  case tile_climate::WARM : std::cout << "Warm"; break;
	  case tile_climate::HOT : std::cout << "Hot"; break;
	  case tile_climate::VERY_HOT : std::cout << "Very Hot"; break;
	  default : std::cout << "Other";
	}
	
	std::cout << ", ";
	switch (biome_type) {
	  case biome_types::TUNDRA : std::cout << "Tundra"; break;
	  case biome_types::DESERT : std::cout << "Desert"; break;
	  case biome_types::JUNGLE : std::cout << "Jungle"; break;
	  case biome_types::BADLANDS : std::cout << "Badlands"; break;
	  case biome_types::SWAMP : std::cout << "Swamp"; break;
	  case biome_types::FLATLAND : std::cout << "Flatland"; break;
	  case biome_types::WOODS : std::cout << "Woods"; break;
	  case biome_types::FOREST : std::cout << "Forest"; break;
	  case biome_types::HILLS : std::cout << "Hills"; break;
	}
	
	std::cout << ", Difficulty: " << difficulty;
	std::cout << "\n";
    }
};

struct biome_map_t {
    std::unique_ptr<heightmap_t> biome_map;
    std::vector<biome_t> biomes;
};

biome_map_t make_biome_map ( heightmap_t* heightmap, engine::random_number_generator * rng, water_level_map_t * water );
