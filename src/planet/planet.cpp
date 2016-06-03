#include "planet.hpp"
#include <fstream>
#include <rltk.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <iostream>

using namespace rltk;

const std::string planet_filename = "world/planet.dat";

void save_planet(const planet_t &planet) {
	std::fstream lbfile(planet_filename, std::ios::out | std::ios::binary);
	boost::iostreams::filtering_stream<boost::iostreams::output> deflate;
	deflate.push(boost::iostreams::zlib_compressor());
    deflate.push(lbfile);
	serialize(deflate, planet.name);
	serialize(deflate, planet.rng_seed);
	serialize(deflate, planet.perlin_seed);
	serialize(deflate, planet.water_height);
	serialize(deflate, planet.plains_height);
	serialize(deflate, planet.hills_height);

	serialize(deflate, planet.biomes.size());
	for (const biome_t &biome : planet.biomes) {
		serialize(deflate, biome.biome_type);
		serialize(deflate, biome.name);
		serialize(deflate, biome.mean_temperature);
		serialize(deflate, biome.climate);
	}

	serialize(deflate, planet.landblocks.size());
	for (const block_t &block : planet.landblocks) {
		serialize(deflate, block.height);
		serialize(deflate, block.type);
		serialize(deflate, block.biome_idx);
	}
}

planet_t load_planet() {
	planet_t planet;

	std::fstream lbfile(planet_filename, std::ios::in | std::ios::binary);
	boost::iostreams::filtering_stream<boost::iostreams::input> inflate;
	inflate.push(boost::iostreams::zlib_decompressor());
    inflate.push(lbfile);
	deserialize(inflate, planet.name);
	deserialize(inflate, planet.rng_seed);
	deserialize(inflate, planet.perlin_seed);
	deserialize(inflate, planet.water_height);
	deserialize(inflate, planet.plains_height);
	deserialize(inflate, planet.hills_height);

	std::size_t number_of_biomes;
	deserialize(inflate, number_of_biomes);
	for (std::size_t i=0; i<number_of_biomes; ++i) {
		biome_t biome;
		deserialize(inflate, biome.biome_type);
		deserialize(inflate, biome.name);
		deserialize(inflate, biome.mean_temperature);
		deserialize(inflate, biome.climate);
		planet.biomes.push_back(biome);
	}

	std::size_t number_of_landblocks;
	deserialize(inflate, number_of_landblocks);
	for (std::size_t i=0; i<number_of_landblocks; ++i) {
		block_t block;
		deserialize(inflate, block.height);
		deserialize(inflate, block.type);
		deserialize(inflate, block.biome_idx);
		planet.landblocks.push_back(block);
	}

	return planet;
}
