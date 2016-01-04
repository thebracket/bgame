#pragma once

#include <cstdint>
#include <string>

namespace tile_type
{
constexpr unsigned char WATER = 1;
constexpr unsigned char FLAT = 2;
constexpr unsigned char RAMP = 3;
constexpr unsigned char BEACH = 4;
constexpr unsigned char RAMP_NU_SD = 5;
constexpr unsigned char RAMP_ND_SU = 6;
constexpr unsigned char RAMP_EU_WD = 7;
constexpr unsigned char RAMP_ED_WU = 8;
constexpr unsigned char RAMP_WU_SU = 9;
constexpr unsigned char RAMP_EU_SU = 10;
constexpr unsigned char RAMP_EU_NU = 11;
constexpr unsigned char RAMP_WU_NU = 12;
constexpr unsigned char RAMP_WD_ND = 13;
constexpr unsigned char RAMP_ED_ND = 14;
constexpr unsigned char RAMP_ED_SD = 15;
constexpr unsigned char RAMP_WD_SD = 16;
constexpr unsigned char MAGMA = 17;
constexpr unsigned char EMPTY_SPACE = 18;
constexpr unsigned char PERMANENT_ICE = 19;
}

inline std::string tile_type_to_string(const unsigned char t)
{
	switch (t)
	{
	case tile_type::WATER:
		return "Water";
	case tile_type::FLAT:
		return "Flat";
	case tile_type::RAMP:
		return "Upwards Ramp";
	case tile_type::BEACH:
		return "Beach";
	case tile_type::MAGMA:
		return "Magma!";
	case tile_type::EMPTY_SPACE:
		return "Empty space";
	case tile_type::PERMANENT_ICE:
		return "Permanent Ice";
	}
	return "Unknown";
}

namespace tile_climate
{
constexpr unsigned char ARCTIC = 1;
constexpr unsigned char SUBARCTIC = 2;
constexpr unsigned char COLD = 3;
constexpr unsigned char COOL = 4;
constexpr unsigned char TEMPERATE = 5;
constexpr unsigned char WARM = 6;
constexpr unsigned char HOT = 7;
constexpr unsigned char VERY_HOT = 8;
constexpr unsigned char SUBTERRANEAN = 9;
constexpr unsigned char MOLTEN = 10;
}

inline std::string climate_type_to_string(const unsigned char t)
{
	switch (t)
	{
	case tile_climate::ARCTIC:
		return "Arctic";
	case tile_climate::SUBARCTIC:
		return "Sub-arctic";
	case tile_climate::COLD:
		return "Cold";
	case tile_climate::COOL:
		return "Cool";
	case tile_climate::TEMPERATE:
		return "Temperate";
	case tile_climate::WARM:
		return "Warm";
	case tile_climate::HOT:
		return "Hot";
	case tile_climate::VERY_HOT:
		return "Very Hot";
	case tile_climate::SUBTERRANEAN:
		return "Subterranean";
	case tile_climate::MOLTEN:
		return "Molten";
	}
	return "Unknown";
}

namespace tile_ground
{
constexpr unsigned char IGNEOUS = 1;
constexpr unsigned char SEDIMENTARY = 2;
constexpr unsigned char GRAVEL = 3;
constexpr unsigned char WHITE_SAND = 4;
constexpr unsigned char YELLOW_SAND = 5;
constexpr unsigned char RED_SAND = 6;
constexpr unsigned char PERMAFROST_WHITE = 7;
constexpr unsigned char PERMAFROST_BLUE = 8;
constexpr unsigned char PERMAFROST_CRACKED = 9;
constexpr unsigned char PERMAFROST_DIRTY = 10;
constexpr unsigned char YELLOW_SAND_RIPPLE = 11;
constexpr unsigned char RED_SAND_RIPPLE = 12;
constexpr unsigned char YELLOW_SAND_DIRTY = 13;
constexpr unsigned char RED_SAND_ROUGH = 14;
constexpr unsigned char MUD = 15;
}

inline std::string ground_type_to_string(const unsigned char t)
{
	switch (t)
	{
	case tile_ground::IGNEOUS:
		return "Igneous Rock";
	case tile_ground::SEDIMENTARY:
		return "Sedimentary Rock";
	case tile_ground::GRAVEL:
		return "Rough Gravel";
	case tile_ground::WHITE_SAND:
		return "White Sand";
	case tile_ground::YELLOW_SAND:
		return "Yellow Sand";
	case tile_ground::RED_SAND:
		return "Red Sand";
	case tile_ground::PERMAFROST_WHITE:
		return "Permafrost";
	case tile_ground::PERMAFROST_BLUE:
		return "Permafrost";
	case tile_ground::PERMAFROST_CRACKED:
		return "Cracked Permafrost";
	case tile_ground::PERMAFROST_DIRTY:
		return "Permafrost and soil";
	case tile_ground::YELLOW_SAND_RIPPLE:
		return "Yellow Sand";
	case tile_ground::RED_SAND_RIPPLE:
		return "Red Sand";
	case tile_ground::RED_SAND_ROUGH:
		return "Red Sand and Gravel";
	case tile_ground::MUD:
		return "Muddy Soil";
	}
	return "Unknown";
}

namespace tile_covering
{
constexpr unsigned char BARE = 0;
constexpr unsigned char MOSS = 1;
constexpr unsigned char LYCHEN = 2;
constexpr unsigned char HEATHER = 3;
constexpr unsigned char GRASS = 4;
constexpr unsigned char SHRUB = 5;
constexpr unsigned char THISTLE = 6;
constexpr unsigned char GORSE = 7;
constexpr unsigned char REEDS = 8;
constexpr unsigned char WILDFLOWER = 9;
constexpr unsigned char CACTUS = 10;
constexpr unsigned char PEBBLES = 11;
constexpr unsigned char GRASS_SPARSE = 12;
}

inline std::string covering_type_to_string(const unsigned char t)
{
	switch (t)
	{
	case tile_covering::BARE:
		return "Bare Ground";
	case tile_covering::MOSS:
		return "Moss";
	case tile_covering::LYCHEN:
		return "Lychen";
	case tile_covering::HEATHER:
		return "Heather";
	case tile_covering::GRASS:
		return "Grass";
	case tile_covering::SHRUB:
		return "Shrub";
	case tile_covering::THISTLE:
		return "Thistle";
	case tile_covering::GORSE:
		return "Gorse";
	case tile_covering::REEDS:
		return "Reeds";
	case tile_covering::WILDFLOWER:
		return "Wild flowers";
	case tile_covering::CACTUS:
		return "Cactus";
	case tile_covering::PEBBLES:
		return "Pebbles";
	case tile_covering::GRASS_SPARSE:
		return "Sparse Grass";
	}
	return "Unknown";
}

namespace biome_types
{
constexpr uint8_t TUNDRA = 1;
constexpr uint8_t DESERT = 2;
constexpr uint8_t JUNGLE = 3;
constexpr uint8_t BADLANDS = 4;
constexpr uint8_t SWAMP = 5;
constexpr uint8_t FLATLAND = 6;
constexpr uint8_t WOODS = 7;
constexpr uint8_t FOREST = 8;
constexpr uint8_t HILLS = 9;
}

inline std::string biome_type_to_string(const unsigned char t)
{
	switch (t)
	{
	case biome_types::TUNDRA:
		return "Tundra";
	case biome_types::DESERT:
		return "Desert";
	case biome_types::JUNGLE:
		return "Jungle";
	case biome_types::BADLANDS:
		return "Badlands";
	case biome_types::SWAMP:
		return "Swamp";
	case biome_types::FLATLAND:
		return "Flatland/Plain";
	case biome_types::WOODS:
		return "Woods";
	case biome_types::FOREST:
		return "Forest";
	case biome_types::HILLS:
		return "Hills";
	}
	return "Unknown";
}
