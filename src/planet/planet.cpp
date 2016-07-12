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

	return planet;
}
