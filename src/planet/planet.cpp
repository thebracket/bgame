#include "planet.hpp"
#include <fstream>
#include <rltk.hpp>
#include <Poco/InflatingStream.h>
#include <Poco/DeflatingStream.h>
#include <iostream>

using namespace rltk;

const std::string planet_filename = "world/planet.dat";

void save_planet(const planet_t &planet) {
	std::fstream masterfile(planet_filename, std::ios::out | std::ios::binary);
	Poco::DeflatingOutputStream lbfile(masterfile, Poco::DeflatingStreamBuf::STREAM_GZIP);
	serialize(lbfile, planet.name);
	serialize(lbfile, planet.rng_seed);
	serialize(lbfile, planet.perlin_seed);
	serialize(lbfile, planet.water_height);
	serialize(lbfile, planet.plains_height);
	serialize(lbfile, planet.hills_height);

	serialize(lbfile, planet.biomes.size());
	for (const biome_t &biome : planet.biomes) {
		serialize(lbfile, biome.biome_type);
		serialize(lbfile, biome.name);
		serialize(lbfile, biome.mean_temperature);
		serialize(lbfile, biome.climate);
	}

	serialize(lbfile, planet.landblocks.size());
	for (const block_t &block : planet.landblocks) {
		serialize(lbfile, block.height);
		serialize(lbfile, block.type);
		serialize(lbfile, block.biome_idx);
	}
}

planet_t load_planet() {
	planet_t planet;

	std::fstream masterfile(planet_filename, std::ios::in | std::ios::binary);
	Poco::InflatingInputStream lbfile(masterfile, Poco::InflatingStreamBuf::STREAM_GZIP);
	deserialize(lbfile, planet.name);
	deserialize(lbfile, planet.rng_seed);
	deserialize(lbfile, planet.perlin_seed);
	deserialize(lbfile, planet.water_height);
	deserialize(lbfile, planet.plains_height);
	deserialize(lbfile, planet.hills_height);

	std::size_t number_of_biomes;
	deserialize(lbfile, number_of_biomes);
	for (std::size_t i=0; i<number_of_biomes; ++i) {
		biome_t biome;
		deserialize(lbfile, biome.biome_type);
		deserialize(lbfile, biome.name);
		deserialize(lbfile, biome.mean_temperature);
		deserialize(lbfile, biome.climate);
		planet.biomes.push_back(biome);
	}

	std::size_t number_of_landblocks;
	deserialize(lbfile, number_of_landblocks);
	for (std::size_t i=0; i<number_of_landblocks; ++i) {
		block_t block;
		deserialize(lbfile, block.height);
		deserialize(lbfile, block.type);
		deserialize(lbfile, block.biome_idx);
		planet.landblocks.push_back(block);
	}

	return planet;
}
