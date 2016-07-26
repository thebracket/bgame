#pragma once

#include <vector>
#include <rltk.hpp>
#include "tile.hpp"
#include "constants.hpp"
#include "../utils/bitset8.hpp"

constexpr std::size_t mapidx(const int x, const int y, const int z) { 
	return (z * REGION_HEIGHT * REGION_WIDTH) + (y * REGION_WIDTH) + x; 
}

struct region_t {
	region_t() { 
		revealed.resize(REGION_TILES_COUNT);
		visible.resize(REGION_TILES_COUNT);
		solid.resize(REGION_TILES_COUNT);
		opaque.resize(REGION_TILES_COUNT);
		tile_type.resize(REGION_TILES_COUNT);
		tile_material.resize(REGION_TILES_COUNT);
		tile_hit_points.resize(REGION_TILES_COUNT);
		building_id.resize(REGION_TILES_COUNT);
		tree_id.resize(REGION_TILES_COUNT);
		tile_vegetation_type.resize(REGION_TILES_COUNT);
		tile_flags.resize(REGION_TILES_COUNT);
		render_cache.resize(REGION_TILES_COUNT);
		water_level.resize(REGION_TILES_COUNT);
		above_ground.resize(REGION_TILES_COUNT);
	}

	int region_x, region_y, biome_idx;
	std::vector<bool> revealed;
	std::vector<bool> visible;
	std::vector<bool> solid;
	std::vector<bool> opaque;

	// New tile format data
	std::vector<uint8_t> tile_type;
	std::vector<std::size_t> tile_material;
	std::vector<uint8_t> tile_hit_points;
	std::vector<std::size_t> building_id;
	std::vector<std::size_t> tree_id;
	std::vector<std::size_t> tile_vegetation_type;
	std::vector<bitset8> tile_flags;
	std::vector<rltk::vchar> render_cache;
	std::vector<uint8_t> water_level;
	std::vector<bool> above_ground;

	void tile_recalc_all();
	void tile_calculate(const int &x, const int &y, const int &z);
	void tile_pathing(const int &x, const int &y, const int &z);
	void calc_render(const int &idx);

	int next_tree_id = 1;
};

void save_region(const region_t &region);
region_t load_region(const int region_x, const int region_y);

