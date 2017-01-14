#include "planet.hpp"
#include <fstream>
#include <rltk.hpp>
//#include <boost/iostreams/filtering_stream.hpp>
//#include <boost/iostreams/filter/zlib.hpp>
#include <iostream>

using namespace rltk;

const std::string planet_filename = "world/planet.dat";

void save_planet(const planet_t &planet) {
	std::fstream deflate(planet_filename, std::ios::out | std::ios::binary);
	//boost::iostreams::filtering_stream<boost::iostreams::output> deflate;
	//deflate.push(boost::iostreams::zlib_compressor());
    //deflate.push(lbfile);
	serialize(deflate, planet.name);
	serialize(deflate, planet.rng_seed);
	serialize(deflate, planet.perlin_seed);
	serialize(deflate, planet.water_height);
	serialize(deflate, planet.plains_height);
	serialize(deflate, planet.hills_height);
	serialize(deflate, planet.remaining_settlers);
	serialize(deflate, planet.migrant_counter);

	serialize(deflate, planet.landblocks.size());
	for (const block_t &block : planet.landblocks) {
		serialize(deflate, block.height);
		serialize(deflate, block.variance);
		serialize(deflate, block.type);
		serialize(deflate, block.temperature_c);
		serialize(deflate, block.rainfall);
		serialize(deflate, block.biome_idx);
	}

	serialize(deflate, planet.biomes.size());
	for (const biome_t &biome : planet.biomes) {
		serialize(deflate, biome.type);
		serialize(deflate, biome.name);
		serialize(deflate, biome.mean_temperature);
		serialize(deflate, biome.mean_rainfall);
		serialize(deflate, biome.mean_altitude);
		serialize(deflate, biome.mean_variance);
		serialize(deflate, biome.warp_mutation);
		serialize(deflate, biome.evil);
		serialize(deflate, biome.savagery);
		serialize(deflate, biome.center_x);
		serialize(deflate, biome.center_y);
	}

	serialize(deflate, planet.rivers.size());
	for (const river_t &river : planet.rivers) {
		serialize(deflate, river.name);
		serialize(deflate, river.start_x);
		serialize(deflate, river.start_y);
		serialize(deflate, river.steps.size());
		for (const river_step_t &s : river.steps) {
			serialize(deflate, s.x);
			serialize(deflate, s.y);
		}
	}

	planet.civs.save(deflate);
	planet.history.save(deflate);
}

planet_t load_planet() {
	planet_t planet;

	std::fstream inflate(planet_filename, std::ios::in | std::ios::binary);
	//boost::iostreams::filtering_stream<boost::iostreams::input> inflate;
	//inflate.push(boost::iostreams::zlib_decompressor());
    //inflate.push(lbfile);
	deserialize(inflate, planet.name);
	deserialize(inflate, planet.rng_seed);
	deserialize(inflate, planet.perlin_seed);
	deserialize(inflate, planet.water_height);
	deserialize(inflate, planet.plains_height);
	deserialize(inflate, planet.hills_height);
	deserialize(inflate, planet.remaining_settlers);
	deserialize(inflate, planet.migrant_counter);

	std::size_t n_blocks;

	deserialize(inflate, n_blocks);
	planet.landblocks.resize(n_blocks);
	for (std::size_t i=0; i<n_blocks; ++i) {
		block_t block;
		deserialize(inflate, block.height);
		deserialize(inflate, block.variance);
		deserialize(inflate, block.type);
		deserialize(inflate, block.temperature_c);
		deserialize(inflate, block.rainfall);
		deserialize(inflate, block.biome_idx);
		planet.landblocks[i] = block;

	}
	
	std::size_t n_biomes;
	deserialize(inflate, n_biomes);
	planet.biomes.resize(n_biomes);
	for (std::size_t i=0; i<n_biomes; ++i) {
		biome_t biome;
		deserialize(inflate, biome.type);
		deserialize(inflate, biome.name);
		deserialize(inflate, biome.mean_temperature);
		deserialize(inflate, biome.mean_rainfall);
		deserialize(inflate, biome.mean_altitude);
		deserialize(inflate, biome.mean_variance);
		deserialize(inflate, biome.warp_mutation);
		deserialize(inflate, biome.evil);
		deserialize(inflate, biome.savagery);
		deserialize(inflate, biome.center_x);
		deserialize(inflate, biome.center_y);

		planet.biomes[i] = biome;
	}

	std::size_t n_rivers;
	deserialize(inflate, n_rivers);
	planet.rivers.resize(n_rivers);
	for (std::size_t i=0; i<n_rivers; ++i) {
		river_t river;
		deserialize(inflate, river.name);
		deserialize(inflate, river.start_x);
		deserialize(inflate, river.start_y);
		std::size_t n_steps;
		deserialize(inflate, n_steps);
		for (std::size_t j=0; j<n_steps; ++j) {
			river_step_t s;
			deserialize(inflate, s.x);
			deserialize(inflate, s.y);
			river.steps.push_back(s);
		}
		planet.rivers[i] = river;
	}

	planet.civs.load(inflate);
	planet.history.load(inflate);

	return planet;
}
