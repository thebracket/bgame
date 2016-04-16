#include "tile_builder.hpp"
#include "../tile_types.hpp"
#include <iostream>

void build_tile_tundra(const biome_t &biome, tile_t *tile, engine::random_number_generator &rng)
{
	int ground_roll = rng.roll_dice(1, 4);
	switch (ground_roll)
	{
	case 1:
		tile->ground = tile_ground::PERMAFROST_BLUE;
		break;
	case 2:
		tile->ground = tile_ground::PERMAFROST_WHITE;
		break;
	case 3:
		tile->ground = tile_ground::PERMAFROST_CRACKED;
		break;
	case 4:
		tile->ground = tile_ground::PERMAFROST_DIRTY;
		break;
	}

	int covering_roll = rng.roll_dice(1, 6);
	switch (covering_roll)
	{
	case 1:
		tile->covering = tile_covering::LYCHEN;
		break;
	case 2:
		tile->covering = tile_covering::LYCHEN;
		break;
	case 3:
		tile->covering = tile_covering::MOSS;
		break;
	default:
		tile->covering = tile_covering::BARE;
		break;
	}

	if (tile->base_tile_type == tile_type::FLAT) {
		if (rng.roll_dice(1,30)==1) tile->tree = tree_potential::PINE;
	}
}

void build_tile_desert(const biome_t &biome, tile_t *tile, engine::random_number_generator &rng)
{
	int ground_roll = rng.roll_dice(1, 10);
	switch (ground_roll)
	{
	case 1:
		tile->ground = tile_ground::YELLOW_SAND;
		break;
	case 2:
		tile->ground = tile_ground::YELLOW_SAND_RIPPLE;
		break;
	case 3:
		tile->ground = tile_ground::YELLOW_SAND_RIPPLE;
		break;
	case 4:
		tile->ground = tile_ground::YELLOW_SAND_DIRTY;
		break;
	case 5:
		tile->ground = tile_ground::RED_SAND;
		break;
	case 6:
		tile->ground = tile_ground::RED_SAND_RIPPLE;
		break;
	default:
		tile->ground = tile_ground::YELLOW_SAND;
		break;
	}

	int covering_roll = rng.roll_dice(1, 40);
	switch (covering_roll)
	{
	case 1:
		tile->covering = tile_covering::CACTUS;
		break;
	case 2:
		tile->covering = tile_covering::PEBBLES;
		break;
	default:
		tile->covering = tile_covering::BARE;
	}

	if (tile->base_tile_type == tile_type::FLAT) {
		if (rng.roll_dice(1,30)==1) tile->tree = tree_potential::PINE;
	}
}

void build_tile_badlands(const biome_t &biome, tile_t *tile, engine::random_number_generator &rng)
{
	int ground_roll = rng.roll_dice(1, 6);
	switch (ground_roll)
	{
	case 1:
		tile->ground = tile_ground::RED_SAND_ROUGH;
		break;
	case 2:
		tile->ground = tile_ground::RED_SAND_RIPPLE;
		break;
	default:
		tile->ground = tile_ground::RED_SAND;
		break;
	}

	int covering_roll = rng.roll_dice(1, 20);
	switch (covering_roll)
	{
	case 1:
		tile->covering = tile_covering::CACTUS;
		break;
	case 2:
		tile->covering = tile_covering::GRASS_SPARSE;
		break;
	case 3:
		tile->covering = tile_covering::PEBBLES;
		break;
	default:
		tile->covering = tile_covering::BARE;
		break;
	}

	if (tile->base_tile_type == tile_type::FLAT) {
		if (rng.roll_dice(1,30)==1) tile->tree = tree_potential::PINE;
	}
}

void build_tile_swamp(const biome_t &biome, tile_t *tile, engine::random_number_generator &rng)
{
	int ground_roll = rng.roll_dice(1, 6);
	switch (ground_roll)
	{
	case 1:
		tile->ground = tile_ground::GRAVEL;
		break;
	case 2:
		tile->ground = tile_ground::SEDIMENTARY;
		break;
	case 3:
		tile->ground = tile_ground::WHITE_SAND;
		break;
	default:
		tile->ground = tile_ground::MUD;
		break;
	}

	int covering_roll = rng.roll_dice(1, 10);
	switch (covering_roll)
	{
	case 1:
		tile->covering = tile_covering::BARE;
		break;
	case 2:
		tile->covering = tile_covering::GORSE;
		break;
	case 3:
		tile->covering = tile_covering::GRASS;
		break;
	case 4:
		tile->covering = tile_covering::GRASS_SPARSE;
		break;
	case 5:
		tile->covering = tile_covering::MOSS;
		break;
	default:
		tile->covering = tile_covering::REEDS;
		break;
	}

	if (tile->base_tile_type == tile_type::FLAT) {
		if (rng.roll_dice(1,12)==1) tile->tree = tree_potential::PINE;
	}
}

void build_tile_flatland(const biome_t &biome, tile_t *tile, engine::random_number_generator &rng)
{
	int ground_roll = rng.roll_dice(1, 6);
	switch (ground_roll)
	{
	case 1:
		tile->ground = tile_ground::MUD;
		break;
	case 2:
		tile->ground = tile_ground::SEDIMENTARY;
		break;
	case 3:
		tile->ground = tile_ground::WHITE_SAND;
		break;
	case 4:
		tile->ground = tile_ground::IGNEOUS;
		break;
	default:
		tile->ground = tile_ground::GRAVEL;
		break;
	}

	int covering_roll = rng.roll_dice(1, 10);
	switch (covering_roll)
	{
	case 1:
		tile->covering = tile_covering::BARE;
		break;
	case 2:
		tile->covering = tile_covering::WILDFLOWER;
		break;
	case 3:
		tile->covering = tile_covering::THISTLE;
		break;
	case 4:
		tile->covering = tile_covering::HEATHER;
		break;
	case 5:
		tile->covering = tile_covering::GRASS_SPARSE;
		break;
	case 6:
		tile->covering = tile_covering::SHRUB;
		break;
	default:
		tile->covering = tile_covering::GRASS;
		break;
	}

	if (tile->base_tile_type == tile_type::FLAT) {
		if (rng.roll_dice(1,8)==1) tile->tree = tree_potential::PINE;
	}
}

void build_tile_woods(const biome_t &biome, tile_t *tile, engine::random_number_generator &rng)
{
	int ground_roll = rng.roll_dice(1, 6);
	switch (ground_roll)
	{
	case 1:
		tile->ground = tile_ground::MUD;
		break;
	case 2:
		tile->ground = tile_ground::SEDIMENTARY;
		break;
	case 3:
		tile->ground = tile_ground::WHITE_SAND;
		break;
	case 4:
		tile->ground = tile_ground::IGNEOUS;
		break;
	default:
		tile->ground = tile_ground::GRAVEL;
		break;
	}

	int covering_roll = rng.roll_dice(1, 10);
	switch (covering_roll)
	{
	case 1:
		tile->covering = tile_covering::BARE;
		break;
	case 2:
		tile->covering = tile_covering::WILDFLOWER;
		break;
	case 3:
		tile->covering = tile_covering::THISTLE;
		break;
	case 4:
		tile->covering = tile_covering::HEATHER;
		break;
	case 5:
		tile->covering = tile_covering::GRASS_SPARSE;
		break;
	case 6:
		tile->covering = tile_covering::SHRUB;
		break;
	default:
		tile->covering = tile_covering::GRASS;
		break;
	}

	if (tile->base_tile_type == tile_type::FLAT) {
		if (rng.roll_dice(1,6)==1) tile->tree = tree_potential::PINE;
	}
}

void build_tile_forest(const biome_t &biome, tile_t *tile, engine::random_number_generator &rng)
{
	int ground_roll = rng.roll_dice(1, 6);
	switch (ground_roll)
	{
	case 1:
		tile->ground = tile_ground::MUD;
		break;
	case 2:
		tile->ground = tile_ground::SEDIMENTARY;
		break;
	case 3:
		tile->ground = tile_ground::WHITE_SAND;
		break;
	case 4:
		tile->ground = tile_ground::IGNEOUS;
		break;
	default:
		tile->ground = tile_ground::GRAVEL;
		break;
	}

	int covering_roll = rng.roll_dice(1, 10);
	switch (covering_roll)
	{
	case 1:
		tile->covering = tile_covering::BARE;
		break;
	case 2:
		tile->covering = tile_covering::WILDFLOWER;
		break;
	case 3:
		tile->covering = tile_covering::THISTLE;
		break;
	case 4:
		tile->covering = tile_covering::HEATHER;
		break;
	case 5:
		tile->covering = tile_covering::GRASS_SPARSE;
		break;
	case 6:
		tile->covering = tile_covering::SHRUB;
		break;
	default:
		tile->covering = tile_covering::GRASS;
		break;
	}

	if (tile->base_tile_type == tile_type::FLAT) {
		if (rng.roll_dice(1,5)==1) tile->tree = tree_potential::PINE;
	}
}

void build_tile_jungle(const biome_t &biome, tile_t *tile, engine::random_number_generator &rng)
{
	int ground_roll = rng.roll_dice(1, 6);
	switch (ground_roll)
	{
	case 1:
		tile->ground = tile_ground::MUD;
		break;
	case 2:
		tile->ground = tile_ground::SEDIMENTARY;
		break;
	case 3:
		tile->ground = tile_ground::WHITE_SAND;
		break;
	case 4:
		tile->ground = tile_ground::IGNEOUS;
		break;
	default:
		tile->ground = tile_ground::GRAVEL;
		break;
	}

	int covering_roll = rng.roll_dice(1, 10);
	switch (covering_roll)
	{
	case 1:
		tile->covering = tile_covering::BARE;
		break;
	case 2:
		tile->covering = tile_covering::WILDFLOWER;
		break;
	case 4:
		tile->covering = tile_covering::GRASS_SPARSE;
		break;
	case 6:
		tile->covering = tile_covering::SHRUB;
		break;
	default:
		tile->covering = tile_covering::GRASS;
		break;
	}

	if (tile->base_tile_type == tile_type::FLAT) {
		if (rng.roll_dice(1,5)==1) tile->tree = tree_potential::PINE;
	}
}

void build_tile_hills(const biome_t &biome, tile_t *tile, engine::random_number_generator &rng)
{
	int ground_roll = rng.roll_dice(1, 8);
	switch (ground_roll)
	{
	case 1:
		tile->ground = tile_ground::MUD;
		break;
	case 2:
		tile->ground = tile_ground::SEDIMENTARY;
		break;
	case 3:
		tile->ground = tile_ground::WHITE_SAND;
		break;
	case 4:
		tile->ground = tile_ground::IGNEOUS;
		break;
	case 5:
		tile->covering = tile_covering::SHRUB;
		break;
	default:
		tile->ground = tile_ground::GRAVEL;
		break;
	}

	int covering_roll = rng.roll_dice(1, 10);
	switch (covering_roll)
	{
	case 1:
		tile->covering = tile_covering::BARE;
		break;
	case 2:
		tile->covering = tile_covering::WILDFLOWER;
		break;
	case 3:
		tile->covering = tile_covering::THISTLE;
		break;
	case 4:
		tile->covering = tile_covering::HEATHER;
		break;
	case 5:
		tile->covering = tile_covering::GRASS_SPARSE;
		break;
	default:
		tile->covering = tile_covering::GRASS;
		break;
	}

	if (tile->base_tile_type == tile_type::FLAT) {
		if (rng.roll_dice(1,10)==1) tile->tree = tree_potential::PINE;
	}
}

void build_tile(const biome_t& biome, tile_t* tile,	engine::random_number_generator &rng)
{
	tile->climate = biome.climate;

	if (tile->water_level > 0)
	{
		if (biome.climate == tile_climate::ARCTIC
				or biome.climate == tile_climate::SUBARCTIC)
		{
			tile->base_tile_type = tile_type::PERMANENT_ICE;
		}
		return;
	}

	switch (biome.biome_type)
	{
	case biome_types::TUNDRA:
		build_tile_tundra(biome, tile, rng);
		break;
	case biome_types::DESERT:
		build_tile_desert(biome, tile, rng);
		break;
	case biome_types::JUNGLE:
		build_tile_jungle(biome, tile, rng);
		break;
	case biome_types::BADLANDS:
		build_tile_badlands(biome, tile, rng);
		break;
	case biome_types::SWAMP:
		build_tile_swamp(biome, tile, rng);
		break;
	case biome_types::FLATLAND:
		build_tile_flatland(biome, tile, rng);
		break;
	case biome_types::WOODS:
		build_tile_woods(biome, tile, rng);
		break;
	case biome_types::FOREST:
		build_tile_forest(biome, tile, rng);
		break;
	case biome_types::HILLS:
		build_tile_hills(biome, tile, rng);
		break;
	default:
		std::cout << "Unknown biome type: " << biome.biome_type << "\n";
	}
}
