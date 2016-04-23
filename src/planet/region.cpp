#include "region.hpp"
#include <rltk.hpp>
#include <string>

using namespace rltk;

void save_region(const region_t &region) {
	std::string region_filename = "world/region_" + std::to_string(region.region_x) + "_" + std::to_string(region.region_y) + ".dat";
	std::fstream lbfile(region_filename, std::ios::out | std::ios::binary);
	serialize(lbfile, region.region_x);
	serialize(lbfile, region.region_y);
	serialize(lbfile, region.biome_idx);
	serialize(lbfile, region.tiles.size());
	for (const tile_t &tile : region.tiles) {
		serialize(lbfile, tile.base_type);
		serialize(lbfile, tile.contents);
		serialize(lbfile, tile.liquid);
		serialize(lbfile, tile.temperature);
		serialize(lbfile, tile.flags);
	}
}

region_t load_region(const int region_x, const int region_y) {
	std::string region_filename = "world/region_" + std::to_string(region_x) + "_" + std::to_string(region_y) + ".dat";
	std::fstream lbfile(region_filename, std::ios::in | std::ios::binary);
	region_t region;

	deserialize(lbfile, region.region_x);
	deserialize(lbfile, region.region_y);
	deserialize(lbfile, region.biome_idx);
	std::size_t number_of_tiles;
	deserialize(lbfile, number_of_tiles);
	for (std::size_t i=0; i<number_of_tiles; ++i) {
		tile_t tile;
		deserialize(lbfile, tile.base_type);
		deserialize(lbfile, tile.contents);
		deserialize(lbfile, tile.liquid);
		deserialize(lbfile, tile.temperature);
		deserialize(lbfile, tile.flags);
		region.tiles[i] = tile;
	}

	return region;
}
