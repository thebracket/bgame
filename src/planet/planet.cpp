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

	serialize(deflate, planet.landblocks);

	serialize(lbfile, planet.biomes.size());
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

	deserialize(inflate, planet.landblocks);
	
	std::size_t n_biomes;
	deserialize(lbfile, n_biomes);
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

	return planet;
}
