#include "region.hpp"
#include <rltk.hpp>
#include <string>
//#include <Poco/InflatingStream.h>
//#include <Poco/DeflatingStream.h>

using namespace rltk;

void save_region(const region_t &region) {
	std::string region_filename = "world/region_" + std::to_string(region.region_x) + "_" + std::to_string(region.region_y) + ".dat";
	std::fstream deflate(region_filename, std::ios::out | std::ios::binary);
	//Poco::DeflatingOutputStream deflate(lbfile, Poco::DeflatingStreamBuf::STREAM_GZIP);
	serialize(deflate, region.region_x);
	serialize(deflate, region.region_y);
	serialize(deflate, region.biome_idx);
	serialize(deflate, region.tiles.size());
	for (const tile_t &tile : region.tiles) {
		serialize(deflate, tile.base_type);
		serialize(deflate, tile.contents);
		serialize(deflate, tile.liquid);
		serialize(deflate, tile.temperature);
		serialize(deflate, tile.flags);
	}
}

region_t load_region(const int region_x, const int region_y) {
	std::string region_filename = "world/region_" + std::to_string(region_x) + "_" + std::to_string(region_y) + ".dat";
	std::fstream inflate(region_filename, std::ios::in | std::ios::binary);
	//Poco::InflatingInputStream inflate(lbfile, Poco::InflatingStreamBuf::STREAM_GZIP);
	region_t region;

	deserialize(inflate, region.region_x);
	deserialize(inflate, region.region_y);
	deserialize(inflate, region.biome_idx);
	std::size_t number_of_tiles;
	deserialize(inflate, number_of_tiles);
	for (std::size_t i=0; i<number_of_tiles; ++i) {
		tile_t tile;
		deserialize(inflate, tile.base_type);
		deserialize(inflate, tile.contents);
		deserialize(inflate, tile.liquid);
		deserialize(inflate, tile.temperature);
		deserialize(inflate, tile.flags);
		region.tiles[i] = tile;
	}

	return region;
}
